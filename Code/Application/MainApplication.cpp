
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

namespace
{
  CaffApp::Model          model;

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

  const uint32_t width = 864;
  const uint32_t height = 486;

}


namespace Terminal {

class Application
{
public:

  explicit Application()
  : m_caffApp("Terminal", width, height, false)
  , m_projectionMatrix(CaffMath::Matrix44Projection(3.142f / 3.f, static_cast<float>(width), static_cast<float>(height), 0.1f, 1000.f))
  , m_viewMatrix(CaffMath::Matrix44InitIdentity())
  , m_worldMatrix(CaffMath::Matrix44InitIdentity())
  {
    model.loadModel(CaffUtil::GetPathDir() + "Models/unit_cube.obj");

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

    textConsoleController->addStringToBuffer("TYPE MOTHERFUCKER!");
    textConsoleController->addStringToInput(">> ");

    auto &input = m_caffApp.getInput();
    
    input.onKeyChangeEvent  = std::bind(&Application::onKeyChange, this, std::placeholders::_1, std::placeholders::_2);
    input.onTextStreamEvent = std::bind(&Application::onTextStream, this, std::placeholders::_1);
    input.setTextStream(true);
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
        auto &renderer = m_caffApp.getRenderer();

        //static float spin = 0.f;
        //spin += deltaTime;
        //
        //const float x = CaffMath::Sin(spin) * 2.f;
        //const float z = CaffMath::Cos(spin) * 2.f;
        //const float y = CaffMath::Sin(spin) * 2.f;

        //CaffMath::Vector3 eye   = CaffMath::Vector3Init(x, 2.f, z);
        //CaffMath::Vector3 look  = CaffMath::Vector3Init(0.f, 0.f, 0.f);
        //CaffMath::Vector3 up    = CaffMath::Vector3Init(0.f, 1.f, 0.f);
        //CaffMath::Matrix44 view = CaffMath::Matrix44LookAt(eye, up, look);

        //// Draw geometry
        //{
        //  textConsoleView->m_frameBuffer.clear(true, true);

        //  CaffApp::Dev::Renderer::Reset();
        //  caffAppShader.setShaderRaw("viewMat",   sizeof(float) * 16, &view._11);
        //  caffAppShader.setTexture("diffuseTex",  caffAppTexture);

        //  //CaffApp::Dev::Renderer::Draw(textConsole->m_frameBuffer, caffAppShader, caffAppVertexFormat, caffAppVertexBuffer);

        //  caffAppFrameBuffer.clear(false, true);

        //}


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

          CaffApp::Dev::Renderer::Draw(m_caffApp.getRenderer(), caffAppPostShader, caffAppPostVertexFormat, caffAppPostVertexBuffer);
        }

        GL_ERROR("End of frame");
      }
      
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
        textConsoleController->addStringToInput(">> ");
      }
    }
  }

private:

  CaffApp::Application    m_caffApp;
  CaffMath::Matrix44      m_projectionMatrix;
  CaffMath::Matrix44      m_viewMatrix;
  CaffMath::Matrix44      m_worldMatrix;

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