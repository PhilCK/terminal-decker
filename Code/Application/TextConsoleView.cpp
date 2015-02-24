
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


TextConsoleView::TextConsoleView(FontData::FontDataInfo fontData)
{
  const std::string filename = CaffUtil::GetPathDir() + "Shaders/Text.shd";
  const std::string textShader(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());

  m_textShader.loadShader(textShader);
  assert(m_textShader.isValid());


  // Size of Screen.
  cols = 105;
  rows = 40;

  sizeOfWidth  = (static_cast<float>(cols) * static_cast<float>(fontData.maxWidth)) * 0.25f;
  sizeOfHeight = (static_cast<float>(rows) * static_cast<float>(fontData.maxHeight)) * 0.25f;

  m_frameBuffer.loadBuffer(sizeOfWidth, sizeOfHeight);

  const float width  = sizeOfWidth;
  const float height = sizeOfHeight;

  m_textShader.setShader1f("uniScreenWidth",  width);
  m_textShader.setShader1f("uniScreenHeight", height);
  
  m_textShader.setShader1f("uniNumberOfCharsWidth",   cols);
  m_textShader.setShader1f("uniNumberOfCharsHeight",  rows);
    
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

  m_texture.loadTexture(CaffUtil::GetPathDir() + "Textures/courier_new_font.png");
  //m_texture.loadTexture(CaffUtil::GetPathDir() + "Textures/dev_font.png");
  assert(m_texture.isValid());

  std::array<float, 2> scale = {{
    2.f / sizeOfWidth,
    2.f / sizeOfHeight,
  }};

  m_textShader.setShader2f("uniScreenScale", scale);

  std::array<float, 2> coordOffset = {{
    scale.at(0) * (-sizeOfWidth * 0.5f),
    scale.at(1) * (+sizeOfHeight * 0.5f),
  }};

  m_textShader.setShader2f("uniCoordOffset", coordOffset);
}

void TextConsoleView::renderTextConsole(FontData::FontDataInfo fontData, const std::string &str)
{
  auto convertToNormalized = [&](const float screenX, const float screenY) -> std::array<float, 2>
  {
    const float scaleX = +2.f / sizeOfWidth;   // 2 because screen is between 1 and -1;
    const float scaleY = -2.f / sizeOfHeight;  // -1 to flip. we want 0,0 top left.
    
    const float normalX = screenX * scaleX - 1.0f;
    const float normalY = screenY * scaleY + 1.0f;

    const std::array<float, 2> resultArray {{normalX, normalY}};

    return resultArray;
  };

  auto getScale = [&](const float defaultScale, const float maxWidth, const float maxHeight, const float currWidth, const float currHeight)
  {
    const float convX = 1.f / sizeOfWidth;
    const float convY = 1.f / sizeOfHeight;

    const float scaleX = currWidth;// * convX; // maxWidth;
    const float scaleY = currHeight;// * convY; // maxHeight;

    const float finalScaleX = scaleX / 2.f;
    const float finalScaleY = scaleY / 2.f;

    const std::array<float, 2> resultArray {{finalScaleX, finalScaleY}};

    return resultArray;
  };

  m_frameBuffer.clear(false, true);
  CaffApp::Dev::Renderer::Reset();

  float posOffset = 78;
  float posOffsetY = 0;

  m_textShader.setTexture("uniFontMap", m_texture);

  const float texSize = 512.f * 2.f;
   std::array<float, 2> texMapScale = {{1.f / texSize, 1.f / texSize}};
   m_textShader.setShader2f("uniTextureMapScale", texMapScale);

  for(const auto &i : str)
  {
    auto currChar = fontData.characters[static_cast<uint16_t>(i)];

    std::array<float, 2> scale = getScale(1.f, fontData.maxWidth, fontData.maxHeight, currChar.width, currChar.height);

    m_textShader.setShader2f("uniCharScale", scale);

    std::array<float, 4> charStart = {{
      currChar.x * texMapScale.at(0),
      currChar.y * texMapScale.at(1),
      (currChar.x * texMapScale.at(0)) + (currChar.width * texMapScale.at(0)),
      (currChar.y * texMapScale.at(1)) + (currChar.height * texMapScale.at(1)),
    }};

    m_textShader.setShader4f("uniTextureCharDetails", charStart);

    //posOffset += (fontData.maxWidth) / 2.f;
    //posOffset += (currChar.width) / 2.f;
    posOffsetY = 84.f + (currChar.height / 2) + (currChar.yOffset);

    //m_textShader.setShader2f("uniCharPosition", convertToNormalized(864.f, 486.f));
    m_textShader.setShader2f("uniCharPosition", convertToNormalized(posOffset, 90.f + posOffsetY));
    
    std::array<float, 2> offsetData = {{
      0.f, // static_cast<float>(currChar.xOffset) * (1.f / sizeOfWidth),
      0.f, //static_cast<float>(currChar.yOffset) * (-1.f / sizeOfHeight),
    }};

    m_textShader.setShader2f("uniCoordOffset", offsetData);

    posOffset += 78;
    //posOffset += fontData.maxWidth;//currChar.xAdvance;//(fontData.maxWidth) / 2;
    //posOffset += (currChar.width) / 2;

    CaffApp::Dev::Renderer::Draw(m_frameBuffer, m_textShader, m_vertexFormat, m_vertexBuffer);

  }

}