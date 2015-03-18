
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
, m_textureLookup(m_model.getPropertyData(), CaffApp::Dev::TextureD::TWO_D, CaffApp::Dev::Format::DEV,  m_model.getSizeOfProperty(),  m_model.getPropertyData().size() / m_model.getSizeOfProperty())
, m_fontLookup(CaffUtil::GetPathDir() + "Textures/Monaco_font.png")

{
  const uint32_t cols = m_model.getColumns();
  const uint32_t rows = m_model.getRows();

  const float sizeOfWidth  = (static_cast<float>(cols) * static_cast<float>(78 / 1.f)) / 4;
  const float sizeOfHeight = (static_cast<float>(rows) * static_cast<float>(78 / 1.f)) / 4;

  m_frameBuffer.loadBuffer(864.f, 486.f);
  //m_frameBuffer.loadBuffer(sizeOfWidth, sizeOfHeight);

  // Text Shader
  {
    const std::string filename = CaffUtil::GetPathDir() + "Shaders/ConsoleTextShader.shd";
    const std::string shader(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());

    m_textShader.loadShader(shader);
    assert(m_textShader.isValid());
  }


  // Console VF
  {
    const std::vector<CaffApp::Dev::AttributeFormatDesc> vertFmtDesc = {
      CaffApp::Dev::AttributeFormatDesc{"inID", CaffApp::Dev::AttrType::FLOAT},
    };

    m_consoleGridVF.loadFormat(vertFmtDesc);
    assert(m_consoleGridVF.hasFormatedLoaded());
  }

  // Generate VBO a collection of points each point is a character.
  {
    const uint32_t size = cols * rows;

    std::vector<float> pointsVBO;
    pointsVBO.reserve(size);

    for(uint32_t i = 0; i < size; ++i)
    {
      //pointsVBO.push_back(m_model.getSizeOfProperty() * i);
      pointsVBO.push_back(i);
    }

    m_consoleGridVBO.loadVertexBuffer(pointsVBO, false);
    assert(m_consoleGridVBO.isValid());
  }

  // Did textures load?
  {
    assert(m_textureLookup.isValid());
    assert(m_fontLookup.isValid());
  }

  // Load some constants
  {
    const std::array<float, 2> screenSize = {{ m_frameBuffer.getWidth(), m_frameBuffer.getHeight() }};
    m_textShader.setShader2f("bufferResolution", screenSize);

    const std::array<float, 2> fontLookupSize = {{ m_fontLookup.getWidth(), m_fontLookup.getHeight() }};
    m_textShader.setShader2f("textureResolution", fontLookupSize);
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
