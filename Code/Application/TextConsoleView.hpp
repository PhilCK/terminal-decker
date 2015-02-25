#ifndef TEXT_CONSOLE_VIEW_INCLUDED_4EC8389D_ADC1_41F8_9279_54A8581E26F1
#define TEXT_CONSOLE_VIEW_INCLUDED_4EC8389D_ADC1_41F8_9279_54A8581E26F1

#include <Application/TextDataParse.hpp>
#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>
#include <Caffeine/Application/Renderer/Shader.hpp>
#include <Caffeine/Application/Renderer/VertexBuffer.hpp>
#include <Caffeine/Application/Renderer/VertexFormat.hpp>
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <string>
#include <vector>


struct ConsoleChar
{
  uint32_t s;
  uint32_t t;
};

struct ConsoleFont
{
  uint32_t charWidth;
  uint32_t charHeight;
};


using FontDesc = std::tuple<ConsoleFont, std::vector<ConsoleChar>>;

FontDesc ConvertFontToConsole(FontData::FontDataInfo fontData);


class TextConsoleView
{
public:

  explicit        TextConsoleView(FontData::FontDataInfo fontData);

  void            addTextToScreenBuffer();
  void            addTextToInputBuffer();
  
  void            getScreenBuffer(const uint32_t linesFromBottom);
  void            getInputBuffer();

  void            renderTextConsole(FontData::FontDataInfo fontData, const std::string &str);

private:

  CaffApp::Dev::Shader          m_textShader;
  CaffApp::Dev::VertexBuffer    m_vertexBuffer;
  CaffApp::Dev::VertexFormat    m_vertexFormat;
  CaffApp::Dev::Texture         m_texture;
  std::vector<std::string>      m_screenBuffer;
  std::vector<std::string>      m_inputHistory; // better container?
  std::string                   m_inputBuffer;

  ConsoleFont                   m_fontDetails;
  std::vector<ConsoleChar>      m_fontCharacters;

  float                         m_fontAdvance = 0;

  
  float sizeOfWidth, sizeOfHeight;
  uint32_t cols, rows;

  CaffApp::Dev::VertexBuffer    m_consoleGridVBO;
  CaffApp::Dev::VertexFormat    m_consoleGridVF;
  CaffApp::Dev::Shader          m_simpleShader;

  public:

  CaffApp::Dev::FrameBuffer     m_frameBuffer;
  

}; // class


#endif // include guard