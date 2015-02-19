
#include <Application/TextConsole.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Application/Renderer/RendererDev.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <fstream>
#include <array>


TextConsole::TextConsole()
{
  const std::string filename = CaffUtil::GetPathDir() + "Shaders/Text.shd";
  const std::string textShader(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());

  m_textShader.loadShader(textShader);
  assert(m_textShader.isValid());

  const float width = 864;
  const float height = 486;
  m_textShader.setShaderRaw("uniScreenWidth",   sizeof(float), &width);
  m_textShader.setShaderRaw("uniScreenHeight",  sizeof(float), &height);
  

  const float numCharWidth = 10;
  const float numCharHeight = 10;
  m_textShader.setShaderRaw("uniNumberOfCharsWidth", sizeof(float), &numCharWidth);
  m_textShader.setShaderRaw("uniNumberOfCharsHeight", sizeof(float), &numCharHeight);
    
  std::vector<GLfloat> point = {{
    -1.f, -1.f, 0.f, 0.f,
    +1.f, -1.f, 1.f, 0.f,
    -1.f, +1.f, 0.f, 1.f,

    +1.f, -1.f, 1.f, 0.f,
    +1.f, +1.f, 1.f, 1.f,
    -1.f, +1.f, 0.f, 1.f,


  }};

  m_vertexBuffer.loadVertexBuffer(point);
  assert(m_vertexBuffer.isValid());

  std::vector<CaffApp::Dev::AttributeFormatDesc> vertDesc = {{
    CaffApp::Dev::AttributeFormatDesc{"inPosition", CaffApp::Dev::AttrType::FLOAT2},
    CaffApp::Dev::AttributeFormatDesc{"inTexC",     CaffApp::Dev::AttrType::FLOAT2},
  }};
  m_vertexFormat.loadFormat(vertDesc);

  m_texture.loadTexture(CaffUtil::GetPathDir() + "Textures/dev_font.png");
  assert(m_texture.isValid());

  std::array<float, 2> scale = {{
    2.f / 864.f,
    2.f / 486.f,
  }};

  m_textShader.setShader2f("uniScreenScale", scale);

  std::array<float, 2> coordOffset = {{
    scale.at(0) * (-864.f * 0.5f),
    scale.at(1) * (+486.f * 0.5f),
  }};


  m_textShader.setShader2f("uniCoordOffset", coordOffset);

}

void TextConsole::renderTextConsole(const CaffApp::Dev::FrameBuffer &frameBuffer)
{
  CaffApp::Dev::Renderer::Reset();

  std::array<float, 2> size = {{
    30.f,
    30.f,
  }};

  m_textShader.setShader2f("uniCharScale", size);

  std::array<float, 2> position = {{
    30.f,
    30.f,
  }};

  m_textShader.setShader2f("uniCharPosition", position);
  m_textShader.setTexture("uniFontMap", m_texture);

  CaffApp::Dev::Renderer::Draw(frameBuffer, m_textShader, m_vertexFormat, m_vertexBuffer);
  //glDisable(GL_DEPTH_TEST);
  //frameBuffer.bind();
  //m_textShader.bind();
  //m_vertexBuffer.bind(m_vertexFormat, m_textShader);

  //glDrawArrays(GL_POINTS, 0, 1);
}