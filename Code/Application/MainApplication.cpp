
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

#include <core/core.hpp>
#include <core/data_core.hpp>
#include <modules/console/console_module.hpp>


namespace
{
  const uint32_t width = 1280;
  const uint32_t height = 720;
  // 1280 × 720
  // 864 x 486


  FontData::FontDataInfo GetFontData(const std::string &filename)
  {
    auto fontData = FontData::ParseData(filename);

    return fontData;
  }

}

namespace Terminal {

class Application
{
public:

  explicit Application()
  : m_caffApp("Terminal", width, height, false)
  , m_textConsoleModel(80, 25, GetFontData("moop"))
  , m_textConsoleView(m_textConsoleModel)
  , m_textConsoleController(m_textConsoleModel)
  , m_luaModel(m_textConsoleController, m_caffApp)
  , m_luaController(m_luaModel)
  , m_sceneModel()
  , m_sceneView(m_sceneModel)
  , m_sceneController(m_sceneModel)
  {
    m_caffApp.getRenderer().setViewPort(width, height);

    // Inital Controller
    {
      m_textConsoleController.addContentToBuffer({32,32,32,32,32,9554,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9557});
      m_textConsoleController.addStringToBuffer("     |Welcome To Remote Console V1.2|");
      m_textConsoleController.addContentToBuffer({32,32,32,32,32,9560,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9563});
      m_textConsoleController.addStringToBuffer("");
      m_textConsoleController.addContentToBuffer({32,32,32,32,32,32,32,32,9617,9618,9619,9608, 32, 65,84,32,89,79,85,82,32,79,87,78,32,82,73,83,75,32, 9608, 9619,9618,9617});
      m_textConsoleController.addStringToBuffer("");
      m_textConsoleController.setPrompt("ReCON:> ");
    }

    // Input
    {
      auto &input = m_caffApp.getInput();
    
      input.onKeyChangeEvent  = std::bind(&Application::onKeyChange, this, std::placeholders::_1, std::placeholders::_2);
      input.onTextStreamEvent = std::bind(&Application::onTextStream, this, std::placeholders::_1);
      input.setTextStream(true);
    }

    // Lua
    {
      m_luaModel.onLoaded();
    }
  }


  void start()
  {
    while(!m_caffApp.shouldQuit())
    {
      m_caffApp.startFrame();

      const float32_t deltaTime = m_caffApp.getDeltaTime();
      
      // Update / Render
      {
        m_caffApp.getInput().setMouseHold(true);

        m_textConsoleModel.prepareData();
        m_luaModel.onUpdate();
        m_sceneController.update(deltaTime, m_caffApp.getInput());

        m_textConsoleView.render(m_caffApp.getRenderer());

        m_caffApp.getRenderer().clear();
        m_sceneView.draw(m_caffApp.getRenderer(), m_textConsoleView.getConsoleFrameBuffer());
      }

      caffcore::think_all_modules();
      caffcore::dispatch_data_notifications();

      m_caffApp.endFrame();
    }
  }


  void onTextStream(const std::string &str)
  {
    m_textConsoleController.addStringToInput(str);

    static std::string dataStr;
    dataStr = str;

    caffcore::DataNode data{0,0,&dataStr[0], sizeof(char) * dataStr.length()};
    caffcore::add_data_to_core(data);
  }


  void onKeyChange(const CaffApp::KeyID id, const CaffApp::KeyState state)
  {
    if(state == CaffApp::KeyState::UP)
    {
      if(id == CaffApp::KeyID::KB_BACKSPACE)
      {
        m_textConsoleController.backspaceInput();
      }

      if(id == CaffApp::KeyID::KB_ENTER || id == CaffApp::KeyID::KB_RETURN)
      {
        m_textConsoleController.addStringToBuffer(m_textConsoleModel.getInput());
        m_luaModel.onCommand(m_textConsoleModel.getInput(), "");

        m_textConsoleController.clearInput();
      }
    }
  }

private:

  CaffApp::Application                    m_caffApp;

  TextConsoleModel                        m_textConsoleModel;
  TextConsoleView                         m_textConsoleView;
  TextConsoleController                   m_textConsoleController;

  LuaModel                                m_luaModel;
  LuaController                           m_luaController;

  SceneModel                              m_sceneModel;
  SceneView                               m_sceneView;
  SceneController                         m_sceneController;

};

} // namespace


void hello()
{
  std::cout << "Hello!" << std::endl;
}

int testHello(lua_State*L)
{
  hello();

  return 0;
}

const luaL_Reg lua_funcs[] = 
{
  {"hello", testHello},
  {0,0},
};


int main(int argc, char **argv)
{
  // Core
  caffcore::push_new_module(ConsoleModule());
  
  caffcore::register_all_modules();
  caffcore::start_all_modules();

  // Logging
  CaffUtil::SetLogLevel(CaffUtil::LogLevel::ERROR_LOG | CaffUtil::LogLevel::INFO_LOG | CaffUtil::LogLevel::WARNING_LOG);
  CaffUtil::SetOutputLogTargets(CaffUtil::LogOutput::CONSOLE);

  Terminal::Application app;
  app.start();

  //lua_State *L = luaL_newstate();
  //luaL_openlibs(L);
  //lua_pushglobaltable(L); 
  //luaL_setfuncs(L, lua_funcs, 0);

  //luaL_dostring(L, "hello()");

  return 0;
}