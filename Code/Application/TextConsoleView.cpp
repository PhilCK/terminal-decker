
#include <Application/TextConsoleView.hpp>
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


TextConsoleView::TextConsoleView(const FontDesc fontDesc)
{
  const std::string filename = CaffUtil::GetPathDir() + "Shaders/Text.shd";
  const std::string textShader(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());

  m_textShader.loadShader(textShader);
  assert(m_textShader.isValid());


  const float width  = 864.f;
  const float height = 486.f;
  m_textShader.setShaderRaw("uniScreenWidth",   sizeof(float), &width);
  m_textShader.setShaderRaw("uniScreenHeight",  sizeof(float), &height);
  
  const float numCharWidth  = 10;
  const float numCharHeight = 10;
  m_textShader.setShaderRaw("uniNumberOfCharsWidth", sizeof(float), &numCharWidth);
  m_textShader.setShaderRaw("uniNumberOfCharsHeight", sizeof(float), &numCharHeight);
    
  std::vector<GLfloat> point = {{
    -1.f, -1.f, 0.f, 1.f,
    +1.f, -1.f, 1.f, 1.f,
    -1.f, +1.f, 0.f, 0.f,

    +1.f, -1.f, 1.f, 1.f,
    +1.f, +1.f, 1.f, 0.f,
    -1.f, +1.f, 0.f, 0.f,
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

void TextConsoleView::renderTextConsole(const CaffApp::Dev::FrameBuffer &frameBuffer, FontData::FontDataInfo fontData, const std::string &str)
{
  auto convertToNormalized = [](const float screenX, const float screenY) -> std::array<float, 2>
  {
    const float scaleX = +2.f / 864.f;   // 2 because screen is between 1 and -1;
    const float scaleY = -2.f / 486.f;  // -1 to flip. we want 0,0 top left.
    
    const float normalX = screenX * scaleX - 1.0f;
    const float normalY = screenY * scaleY + 1.0f;

    const std::array<float, 2> resultArray {{normalX, normalY}};

    return resultArray;
  };

  auto getScale = [](const float defaultScale, const float maxWidth, const float maxHeight, const float currWidth, const float currHeight)
  {
    const float scaleX = currWidth / maxWidth;
    const float scaleY = currHeight / maxHeight;

    const float finalScaleX = scaleX * defaultScale;
    const float finalScaleY = scaleY * defaultScale;

    const std::array<float, 2> resultArray {{currWidth, currHeight}};

    return resultArray;
  };

  CaffApp::Dev::Renderer::Reset();

  float posOffset = 0;

  for(const auto &i : str)
  {
    auto currChar = fontData.characters[static_cast<uint16_t>(i)];

    std::array<float, 2> scale = getScale(1.f, fontData.maxWidth, fontData.maxHeight, currChar.width, currChar.height);

    m_textShader.setShader2f("uniCharScale", scale);

    m_textShader.setTexture("uniFontMap", m_texture);

    std::array<float, 2> texMapScale = {{1.f / 512.f, 1.f / 512.f}};

    m_textShader.setShader2f("uniTextureMapScale", texMapScale);

    std::array<float, 4> charStart = {{
      currChar.x * texMapScale.at(0),
      currChar.y * texMapScale.at(1),
      (currChar.x * texMapScale.at(0)) + (currChar.width * texMapScale.at(0)),
      (currChar.y * texMapScale.at(1)) + (currChar.height * texMapScale.at(1)),
    }};

    m_textShader.setShader4f("uniTextureCharDetails", charStart);

    posOffset += ((currChar.width));

    //m_textShader.setShader2f("uniCharPosition", convertToNormalized(864.f, 486.f));
    m_textShader.setShader2f("uniCharPosition", convertToNormalized(30.f + posOffset, 30.f));
    
    posOffset += ((currChar.width));

    CaffApp::Dev::Renderer::Draw(frameBuffer, m_textShader, m_vertexFormat, m_vertexBuffer);
    //glDisable(GL_DEPTH_TEST);
    //frameBuffer.bind();
    //m_textShader.bind();
    //m_vertexBuffer.bind(m_vertexFormat, m_textShader);

    //glDrawArrays(GL_POINTS, 0, 1);
    //posOffset += currChar.width;
  }

}