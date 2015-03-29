
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

#include <Application/Scene/SceneController.hpp>
#include <Application/Scene/SceneView.hpp>
#include <Application/Scene/SceneModel.hpp>

#include <lua.hpp>
#include <Lua/LuaController.hpp>
#include <Lua/LuaModel.hpp>
#include <vector>


namespace
{
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
  {
    sceneModel.reset(new SceneModel());
    sceneController.reset(new SceneController(*sceneModel));
    sceneView.reset(new SceneView(*sceneModel));

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
          textConsoleView->renderTextConsole(m_caffApp.getRenderer());
        }

        GL_ERROR("End of frame");
      }
  
      {
        auto &renderer = m_caffApp.getRenderer();
        sceneView->draw(renderer, textConsoleView->m_finalOutput);
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

  CaffApp::Application                    m_caffApp;

  std::unique_ptr<LuaController>          luaController;
  std::unique_ptr<LuaModel>               luaModel;

  std::unique_ptr<TextConsoleModel>       textConsoleModel;
  std::unique_ptr<TextConsoleController>  textConsoleController;
  std::unique_ptr<TextConsoleView>        textConsoleView;

  std::unique_ptr<SceneController>        sceneController;
  std::unique_ptr<SceneModel>             sceneModel;
  std::unique_ptr<SceneView>              sceneView;

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