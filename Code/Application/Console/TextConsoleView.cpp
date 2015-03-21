
#include <Application/Console/TextConsoleView.hpp>
#include <Application/Console/TextConsoleModel.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Application/Renderer/RendererDev.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <fstream>
#include <array>


FontDesc ConvertFontToConsole(FontData::FontDataInfo fontData)
{
  ConsoleFont font;
  std::vector<ConsoleChar> chars;

  FontDesc desc {font, chars};

  return desc;
}


TextConsoleView::TextConsoleView(const TextConsoleModel &model)
: m_model(model)
, m_textureLookup(m_model.getPropertyData(), CaffApp::Dev::TextureD::TWO_D, CaffApp::Dev::Format::DEV,  m_model.getSizeOfProperty(),  4096)
, m_fontLookup(CaffUtil::GetPathDir() + "Textures/Monaco_font.png")
{
  const uint32_t cols = m_model.getColumns();
  const uint32_t rows = m_model.getRows();

  // Size of framebuffer
  {
    const uint32_t sizeOfWidth  = cols * m_model.getMaxCharWidth();
    const uint32_t sizeOfHeight = rows * m_model.getLineHeight();

    m_frameBuffer.loadBuffer(sizeOfWidth, sizeOfHeight);
    assert(m_frameBuffer.isValid());
  }

  // Text Shader
  {
    const std::string filename = CaffUtil::GetPathDir() + "Shaders/ConsoleTextShader.shd";
    const std::string shader(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());

    m_textShader.loadShader(shader);
    assert(m_textShader.isValid());
  }

  // Console VertexFormat
  {
    m_consoleGridVF.loadFormat({
      CaffApp::Dev::AttributeFormatDesc{"inID", CaffApp::Dev::AttrType::FLOAT}, // TODO: Make this an int
    });

    assert(m_consoleGridVF.hasFormatedLoaded());
  }

  // Generate VBO a collection of points each point is a character.
  {
    const uint32_t size = cols * rows;

    std::vector<float> pointsVBO; // TODO: Make this an int
    pointsVBO.reserve(size);

    for(uint32_t i = 0; i < size; ++i)
    {
      pointsVBO.push_back(i);
    }

    m_consoleGridVBO.loadVertexBuffer(pointsVBO, false);
    assert(m_consoleGridVBO.isValid());
  }

  // Load some constants
  {
    m_textShader.setShader2f("bufferResolution",  {{ static_cast<float>(m_frameBuffer.getWidth()), static_cast<float>(m_frameBuffer.getHeight()) }});
    m_textShader.setShader2f("textureResolution", {{ static_cast<float>(m_fontLookup.getWidth()), static_cast<float>(m_fontLookup.getHeight()) }});
  }

  // Did textures load?
  {
    assert(m_textureLookup.isValid());
    assert(m_fontLookup.isValid());
  }
}


void TextConsoleView::renderTextConsole()
{
  // Simple shader.
  CaffApp::Dev::Renderer::Reset();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE); // TODO: Done UV's backwards :/

  m_textureLookup.updateSubset(m_model.getPropertyData(), 0, 0);

  m_textShader.setTexture("fontLookup", m_fontLookup);
  m_textShader.setTexture("dataLookup", m_textureLookup);

  m_frameBuffer.bind();
  m_textShader.bind();
  m_consoleGridVBO.bind(m_consoleGridVF, m_textShader);
  
  glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_model.getNumberOfCharactersInData()));
}
