
#include <Application/Console/TextConsoleView.hpp>
#include <Application/Console/TextConsoleModel.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Application/Renderer/RendererDev.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <Caffeine/Application/Renderer/Device.hpp>
#include <fstream>
#include <array>


namespace
{
  inline std::vector<float> GetFullScreenVerts()
  {
    std::vector<float> verts =
    {
      -1.f, -1.f, 0.f, 0.f,
      +3.f, -1.f, 2.f, 0.f,
      -1.f, +3.f, 0.f, 2.f,
    };

    return verts;
  }

  inline std::vector<CaffApp::Dev::AttributeFormatDesc> GetPostVF()
  {
     const std::vector<CaffApp::Dev::AttributeFormatDesc> vertDesc =
     {
        CaffApp::Dev::AttributeFormatDesc{"position", CaffApp::Dev::AttrType::FLOAT2},
        CaffApp::Dev::AttributeFormatDesc{"texcoord", CaffApp::Dev::AttrType::FLOAT2},
     };

     return vertDesc;
  }
  
  std::vector<float> DummyVector()
  {
    std::vector<float> vec;
    vec.reserve((4096 * 4) * 4);
    
    return vec;
  }
}


FontDesc ConvertFontToConsole(FontData::FontDataInfo fontData)
{
  ConsoleFont font;
  std::vector<ConsoleChar> chars;

  FontDesc desc {font, chars};

  return desc;
}


TextConsoleView::TextConsoleView(const uint32_t cols, const uint32_t rows, const uint32_t size_of_text_property, const float max_char_width, const float max_char_height)
: m_textureLookup(DummyVector(), CaffApp::Dev::TextureD::TWO_D, CaffApp::Dev::Format::DEV, 16 / 4,  4096)
, m_fontLookup(CaffUtil::GetPathDir() + "Textures/Monaco_font.png")
, m_postShader(CaffApp::Dev::ShaderUtil::GetShaderCodeFromFile(CaffUtil::GetPathDir() + "Shaders/Post.shd"))
, m_postVBO(GetFullScreenVerts())

, m_postVF(GetPostVF())
, m_finalOutput()
{
  assert(m_postShader.isValid());
  assert(m_postVBO.isValid());

  // Size of framebuffer
  {
    const uint32_t sizeOfWidth  = cols * max_char_width;
    const uint32_t sizeOfHeight = rows * max_char_height;
  
    m_frameBuffer.loadBuffer(sizeOfWidth, sizeOfHeight);
    assert(m_frameBuffer.isValid());

    m_finalOutput.loadBuffer(sizeOfWidth, sizeOfHeight);
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


CaffApp::Dev::FrameBuffer& TextConsoleView::render(TextConsoleModel &model, CaffApp::Dev::Device &renderer)
{
  // Console
  {
    m_frameBuffer.clear();

    // Simple shader.
    CaffApp::Dev::Renderer::Reset();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE); // TODO: Done UV's backwards :/

    m_textureLookup.updateSubset(model.getPropertyData(), 0, 0);

    m_textShader.setTexture("fontLookup", m_fontLookup);
    m_textShader.setTexture("dataLookup", m_textureLookup);

    m_frameBuffer.bind();
    m_textShader.bind();
    m_consoleGridVBO.bind(m_consoleGridVF, m_textShader);
  
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(model.getNumberOfCharactersInData()));
  } // Console

  // Post
  {
    CaffApp::Dev::Renderer::Reset();
            
    static float frameTime = 0;
    frameTime += 0.1f;

    glDisable(GL_DEPTH_TEST);
          
    m_postShader.setShader1f("frameTime", frameTime);
    m_postShader.setShader2f("screenSize", {{ static_cast<float>(m_finalOutput.getWidth()), static_cast<float>(m_finalOutput.getHeight()) }});
    m_postShader.setTexture("texFramebuffer", m_frameBuffer);

    CaffApp::Dev::Renderer::Draw(m_finalOutput, m_postShader, m_postVF, m_postVBO);
  } // Post
  
  return m_finalOutput;
}
