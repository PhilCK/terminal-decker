
#include <Caffeine/Application/Application.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Math/Matrix.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>

#include <fstream>
#include <streambuf> 
#include <Caffeine/Application/Renderer/Shader.hpp>
#include <Caffeine/Application/Renderer/VertexFormat.hpp>
#include <Caffeine/Application/Renderer/VertexBuffer.hpp>
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Application/Renderer/Device.hpp>
#include <Caffeine/Application/Renderer/RendererDev.hpp>
#include <Caffeine/Application/Input.hpp>

#include <Application/Console/TextConsoleModel.hpp>
#include <Application/Console/TextConsoleView.hpp>
#include <Application/Console/TextConsoleController.hpp>
#include <Application/Console/TextDataParse.hpp>

#include <lua.hpp>
#include <Lua/LuaController.hpp>
#include <Lua/LuaModel.hpp>
#include <vector>

namespace
{
  CaffApp::Model          model;
  CaffApp::Model          screen;
  CaffApp::Dev::VertexBuffer  screenBuffer;

  CaffApp::Dev::Shader        caffAppShader;
  CaffApp::Dev::Shader        caffAppPostShader;
  CaffApp::Dev::VertexFormat  caffAppVertexFormat;
  CaffApp::Dev::VertexFormat  caffAppPostVertexFormat;
  CaffApp::Dev::Texture       caffAppTexture;
  CaffApp::Dev::VertexBuffer  caffAppVertexBuffer;
  CaffApp::Dev::VertexBuffer  caffAppPostVertexBuffer;
  CaffApp::Dev::FrameBuffer   caffAppFrameBuffer;
  
  std::unique_ptr<TextConsoleModel> textConsoleModel;
  std::unique_ptr<TextConsoleController> textConsoleController;
  std::unique_ptr<TextConsoleView> textConsoleView;

  std::vector<float> fullscreenVerts = {{
    -1.f, -1.f, 0.f, 0.f,
    +3.f, -1.f, 2.f, 0.f,
    -1.f, +3.f, 0.f, 2.f,
  }};

  const uint32_t width = 1280;
  const uint32_t height = 720;
  // 1280 × 720
  // 864 x 486
}


namespace Terminal {

class Application
{
public:

  explicit Application()
  : m_caffApp("Terminal", width, height, false)
  , m_projectionMatrix(CaffMath::Matrix44Projection(3.142f / 5.f, static_cast<float>(width), static_cast<float>(height), 0.1f, 1000.f))
  , m_viewMatrix(CaffMath::Matrix44InitIdentity())
  , m_worldMatrix(CaffMath::Matrix44InitIdentity())
  , laptopBody(CaffUtil::GetPathDir() + "Textures/laptop_body.png")
  {
    model.loadModel(CaffUtil::GetPathDir() + "Models/laptop.obj");
    screen.loadModel(CaffUtil::GetPathDir() + "Models/laptop_screen.obj");
    screenBuffer.loadVertexBuffer(screen.getMesh(0).getGLVertexBuffer());
    // Nooo Renderer stuff
    {
      const std::string filename = CaffUtil::GetPathDir() + "Shaders/Fullbright.shd";
      const std::string shaderFullbright(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());
      caffAppShader.loadShader(shaderFullbright);
    }

    {
      const std::string filename = CaffUtil::GetPathDir() + "Shaders/Post.shd";
      const std::string shaderPost(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());
      caffAppPostShader.loadShader(shaderPost);
    }
    
    std::vector<CaffApp::Dev::AttributeFormatDesc> vertDesc;
    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"inPosition", CaffApp::Dev::AttrType::FLOAT3});
    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"inTexC", CaffApp::Dev::AttrType::FLOAT2});
    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"inNormal", CaffApp::Dev::AttrType::FLOAT3});

    caffAppVertexFormat.loadFormat(vertDesc);

    vertDesc.clear();

    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"position", CaffApp::Dev::AttrType::FLOAT2});
    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"texcoord", CaffApp::Dev::AttrType::FLOAT2});

    caffAppPostVertexFormat.loadFormat(vertDesc);

    caffAppTexture.loadTexture(CaffUtil::GetPathDir() + "Textures/dev_grid_blue.png");

    caffAppVertexBuffer.loadVertexBuffer(model.getMesh(0).getGLVertexBuffer());
    caffAppPostVertexBuffer.loadVertexBuffer(fullscreenVerts);

    caffAppFrameBuffer.loadBuffer(width, height);

    caffAppShader.setShaderRaw("projMat",   sizeof(float) * 16, &m_projectionMatrix._11);
    caffAppShader.setShaderRaw("worldMat",  sizeof(float) * 16, &m_worldMatrix._11);

    

    m_caffApp.getRenderer().setViewPort(width, height);

    // Text MVC
    {
      std::string filename = "moop";
      auto fontData = FontData::ParseData(filename);

      //auto fontData2 = ConvertFontToConsole(FontData::ParseData(filename));

      textConsoleModel.reset(new TextConsoleModel(80, 25, fontData));
      textConsoleView.reset(new TextConsoleView(*textConsoleModel));
      textConsoleController.reset(new TextConsoleController(*textConsoleModel));
    }

    textConsoleController->addContentToBuffer({32,32,32,32,32,9554,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9557});
    textConsoleController->addStringToBuffer("     |Welcome To Remote Console V1.2|");
    textConsoleController->addContentToBuffer({32,32,32,32,32,9560,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9563});
    textConsoleController->addStringToBuffer("");
    textConsoleController->addContentToBuffer({32,32,32,32,32,32,32,32,9617,9618,9619,9608, 32, 65,84,32,89,79,85,82,32,79,87,78,32,82,73,83,75,32, 9608, 9619,9618,9617});
    textConsoleController->addStringToBuffer("");
    textConsoleController->setPrompt("ReCON:> ");

    auto &input = m_caffApp.getInput();
    
    input.onKeyChangeEvent  = std::bind(&Application::onKeyChange, this, std::placeholders::_1, std::placeholders::_2);
    input.onTextStreamEvent = std::bind(&Application::onTextStream, this, std::placeholders::_1);
    input.setTextStream(true);

    // Lua
    {
      luaModel.reset(new LuaModel());
      luaController.reset(new LuaController(*luaModel));

      luaModel->onLoaded();
    }
  }


  void start()
  {
    std::string filename = "moop";
    auto fontData = FontData::ParseData(filename);    

    while(!m_caffApp.shouldQuit())
    {
      m_caffApp.startFrame();

      const float32_t deltaTime = m_caffApp.getDeltaTime();
      
      // Render
      {
        {
          textConsoleModel->prepareData();

          textConsoleView->m_frameBuffer.clear();
          textConsoleView->renderTextConsole();
        }

        // Draw post
        {
          CaffApp::Dev::Renderer::Reset();
          m_caffApp.getRenderer().setViewPort(width, height);
          
          static float frameTime = 0;
          frameTime += deltaTime;

          glDisable(GL_DEPTH_TEST);
          
          caffAppPostShader.setShader1f("frameTime", frameTime);
          caffAppPostShader.setShader2f("screenSize", {{ width, height }});
          caffAppPostShader.setTexture("texFramebuffer", textConsoleView->m_frameBuffer);

          //CaffApp::Dev::Renderer::Draw(m_caffApp.getRenderer(), caffAppPostShader, caffAppPostVertexFormat, caffAppPostVertexBuffer);
        }

        GL_ERROR("End of frame");
      }

      {
        caffAppShader.setShaderRaw("viewMat", sizeof(m_viewMatrix), &m_viewMatrix._11);
        caffAppShader.setShaderRaw("worldMat", sizeof(m_worldMatrix), &m_worldMatrix._11);
        caffAppShader.setShaderRaw("projMat", sizeof(m_projectionMatrix), &m_projectionMatrix._11);

        //CaffApp::Dev::Renderer::Draw(m_caffApp.getRenderer(), caffAppShader, caffAppVertexFormat, caffAppVertexBuffer);
      }

      {
        auto &renderer = m_caffApp.getRenderer();

        static float spin = CaffMath::QuartTau();
        //spin += deltaTime;
        //
        const float x = CaffMath::Sin(spin) * 2.f;
        const float z = CaffMath::Cos(spin) * 2.f;
        const float y = CaffMath::Sin(spin) * 2.f;

        CaffMath::Vector3 eye   = CaffMath::Vector3Init(x, 2.f, z);
        CaffMath::Vector3 look  = CaffMath::Vector3Init(0.f, 1.f, 0.f);
        CaffMath::Vector3 up    = CaffMath::Vector3Init(0.f, 1.f, 0.f);
        CaffMath::Matrix44 view = CaffMath::Matrix44LookAt(eye, up, look);

        //CaffApp::Dev::Renderer::Draw(m_caffApp.getRenderer(), caffAppShader, caffAppVertexFormat, caffAppVertexBuffer);

        //// Draw geometry
        //{
        //  textConsoleView->m_frameBuffer.clear(true, true);

        //  CaffApp::Dev::Renderer::Reset();
        caffAppShader.setShaderRaw("viewMat",   sizeof(float) * 16, &view._11);
        caffAppShader.setTexture("diffuseTex",  laptopBody);
        CaffApp::Dev::Renderer::Draw(m_caffApp.getRenderer(), caffAppShader, caffAppVertexFormat, caffAppVertexBuffer);

        caffAppShader.setTexture("diffuseTex", textConsoleView->m_frameBuffer);
        CaffApp::Dev::Renderer::Draw(m_caffApp.getRenderer(), caffAppShader, caffAppVertexFormat, screenBuffer);

       //CaffApp::Dev::Renderer::Draw(textConsole->m_frameBuffer, caffAppShader, caffAppVertexFormat, caffAppVertexBuffer);

        //  caffAppFrameBuffer.clear(false, true);

        //}
      }

      luaModel->onUpdate();
      
      m_caffApp.endFrame();
    }
  }


  void onTextStream(const std::string &str)
  {
    textConsoleController->addStringToInput(str);
  }


  void onKeyChange(const CaffApp::KeyID id, const CaffApp::KeyState state)
  {
    if(state == CaffApp::KeyState::UP)
    {
      if(id == CaffApp::KeyID::KB_BACKSPACE)
      {
        textConsoleController->backspaceInput();
      }

      if(id == CaffApp::KeyID::KB_ENTER || id == CaffApp::KeyID::KB_RETURN)
      {
        textConsoleController->addStringToBuffer(textConsoleModel->getInput());
        textConsoleController->clearInput();
      }
    }
  }

private:

  CaffApp::Application    m_caffApp;
  CaffMath::Matrix44      m_projectionMatrix;
  CaffMath::Matrix44      m_viewMatrix;
  CaffMath::Matrix44      m_worldMatrix;
  CaffApp::Dev::Texture   laptopBody;

  std::unique_ptr<LuaController> luaController;
  std::unique_ptr<LuaModel> luaModel;

};

} // namespace


int main(int argc, char **argv)
{
  // Logging
  CaffUtil::SetLogLevel(CaffUtil::LogLevel::ERROR_LOG | CaffUtil::LogLevel::INFO_LOG | CaffUtil::LogLevel::WARNING_LOG);
  CaffUtil::SetOutputLogTargets(CaffUtil::LogOutput::CONSOLE);

  Terminal::Application app;
  app.start();

  return 0;
}