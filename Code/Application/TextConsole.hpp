#ifndef TEXT_CONSOLE_INCLUDED_4EC8389D_ADC1_41F8_9279_54A8581E26F1
#define TEXT_CONSOLE_INCLUDED_4EC8389D_ADC1_41F8_9279_54A8581E26F1

#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>
#include <Caffeine/Application/Renderer/Shader.hpp>
#include <Caffeine/Application/Renderer/VertexBuffer.hpp>
#include <Caffeine/Application/Renderer/VertexFormat.hpp>
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <string>
#include <vector>

class TextConsole
{
public:

  explicit        TextConsole();

  void            addTextToScreenBuffer();
  void            addTextToInputBuffer();
  
  void            getScreenBuffer(const uint32_t linesFromBottom);
  void            getInputBuffer();

  void            renderTextConsole(const CaffApp::Dev::FrameBuffer &frameBuffer);

private:

  CaffApp::Dev::Shader          m_textShader;
  CaffApp::Dev::VertexBuffer    m_vertexBuffer;
  CaffApp::Dev::VertexFormat    m_vertexFormat;
  CaffApp::Dev::Texture         m_texture;
  std::vector<std::string>      m_screenBuffer;
  std::vector<std::string>      m_inputHistory; // better container?
  std::string                   m_inputBuffer;
  

}; // class


#endif // include guard