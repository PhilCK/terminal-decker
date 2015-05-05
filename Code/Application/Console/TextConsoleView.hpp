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


class text_console_view
{

	text_console_view(const text_console_view&) = delete;
	text_console_view& operator=(const text_console_view&) = delete;

public:

  explicit                      text_console_view(const uint32_t cols, const uint32_t rows, const uint32_t size_of_text_property, const float max_char_width, const float max_char_height);

  CaffApp::Dev::FrameBuffer&    render(TextConsoleModel &model, CaffApp::Dev::Device &renderer);

private:


  CaffApp::Dev::VertexBuffer    m_consoleGridVBO;
  CaffApp::Dev::VertexBuffer    m_postVBO = CaffApp::Dev::VertexBuffer({-1.f, -1.f, 0.f, 0.f, +3.f, -1.f, 2.f, 0.f, -1.f, +3.f, 0.f, 2.f,});
  CaffApp::Dev::VertexFormat    m_consoleGridVF = CaffApp::Dev::VertexFormat({
      CaffApp::Dev::AttributeFormatDesc{"inID", CaffApp::Dev::AttrType::FLOAT}, // TODO: Make this an int
    });
  CaffApp::Dev::VertexFormat    m_postVF = CaffApp::Dev::VertexFormat({
        CaffApp::Dev::AttributeFormatDesc{"position", CaffApp::Dev::AttrType::FLOAT2},
        CaffApp::Dev::AttributeFormatDesc{"texcoord", CaffApp::Dev::AttrType::FLOAT2},
     });
  CaffApp::Dev::Shader          m_textShader = CaffApp::Dev::ShaderUtil::create_shader_from_file_name(CaffUtil::GetPathDir() + "Shaders/ConsoleTextShader.shd");
  CaffApp::Dev::Shader          m_postShader = CaffApp::Dev::ShaderUtil::create_shader_from_file_name(CaffUtil::GetPathDir() + "Shaders/Post.shd");
  CaffApp::Dev::Texture         m_textureLookup;
  CaffApp::Dev::Texture         m_fontLookup = CaffApp::Dev::Texture(CaffUtil::GetPathDir() + "Textures/Monaco_font.png");
  CaffApp::Dev::FrameBuffer     m_frameBuffer;
  CaffApp::Dev::FrameBuffer     m_finalOutput;


}; // class


#endif // include guard