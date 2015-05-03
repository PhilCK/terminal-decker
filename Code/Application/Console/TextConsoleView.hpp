#ifndef TEXT_CONSOLE_VIEW_INCLUDED_4EC8389D_ADC1_41F8_9279_54A8581E26F1
#define TEXT_CONSOLE_VIEW_INCLUDED_4EC8389D_ADC1_41F8_9279_54A8581E26F1

#include <Application/Console/TextConsoleFwd.hpp>
#include <Application/Console/TextDataParse.hpp>
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

  explicit                      TextConsoleView(const uint32_t cols, const uint32_t rows, const uint32_t size_of_text_property, const float max_char_width, const float max_char_height);

  CaffApp::Dev::FrameBuffer&    render(TextConsoleModel &model, CaffApp::Dev::Device &renderer);


private:
  
  //const TextConsoleModel        &m_model;

  CaffApp::Dev::VertexBuffer    m_consoleGridVBO;
  CaffApp::Dev::VertexBuffer    m_postVBO;
  CaffApp::Dev::VertexFormat    m_consoleGridVF;
  CaffApp::Dev::VertexFormat    m_postVF;
  CaffApp::Dev::Shader          m_textShader;
  CaffApp::Dev::Shader          m_postShader;
  CaffApp::Dev::Texture         m_textureLookup;
  CaffApp::Dev::Texture         m_fontLookup;

  CaffApp::Dev::FrameBuffer     m_frameBuffer;
  CaffApp::Dev::FrameBuffer     m_finalOutput;

  

}; // class


#endif // include guard