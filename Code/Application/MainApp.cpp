﻿
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
#include <Caffeine/Application/input.hpp>

#include <Application/Console/TextConsoleModel.hpp>
#include <Application/Console/TextConsoleView.hpp>
#include <Application/Console/TextDataParse.hpp>

#include <Application/Scene/SceneController.hpp>
#include <Application/Scene/SceneView.hpp>
#include <Application/Scene/SceneModel.hpp>

#include <vector>

#include <Application/Console/console_screen_controller.hpp>

#include <Application/terminal/terminal.hpp>

#include <Caffeine/Application/audio/audio_manager.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <assimp/Include/assimp/Importer.hpp>
//#include <assimp/Include/assimp/scene.h>
//#include <assimp/Include/assimp/postprocess.h>

#include <Application/Laptop/Laptop.hpp>
#include <Application/Network/Network.hpp>



namespace
{
  const uint32_t width = 960;
  const uint32_t height = 540;
  // 1280 × 720
  // 864 x 486
}


class application
{
public:

  explicit application()
  : m_caffApp("Terminal", width, height, false)
  , m_laptop(4)
  , m_sceneModel()
  , m_sceneView(m_sceneModel)
  , m_sceneController(m_sceneModel)
  {
    m_caffApp.getRenderer().setViewPort(width, height);
    
    const std::string& pFile = CaffUtil::GetPathDir() + "/Models/unit_cube.obj";
    
    auto content = std::vector<uint32_t>({1,2,3,4,5,6,7});
    m_laptopControl.add_content(content);
    
    Assimp::Importer importer;
    
      const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
  
    // If the import failed, report it
    if( !scene)
    /*if( !scene)
    {
    
    }

	*/
    // Inital Controller
    {
      //m_textConsoleController.addContentToBuffer({32,32,32,32,32,9554,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9557});
      //m_textConsoleController.addStringToBuffer("     |Welcome To Remote Console V1.2|");
      //m_textConsoleController.addContentToBuffer({32,32,32,32,32,9560,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9552,9563});
      //m_textConsoleController.addStringToBuffer("");
      //m_textConsoleController.addContentToBuffer({32,32,32,32,32,32,32,32,9617,9618,9619,9608, 32, 65,84,32,89,79,85,82,32,79,87,78,32,82,73,83,75,32, 9608, 9619,9618,9617});
      //m_textConsoleController.addStringToBuffer("");
      //m_textConsoleController.setPrompt("ReCON:> ");
    }

    // Input
    {
      auto &input = m_caffApp.getInput();
    
      input.on_key_change_event  = std::bind(&application::onKeyChange, this, std::placeholders::_1, std::placeholders::_2);
      input.on_text_stream_event = std::bind(&application::onTextStream, this, std::placeholders::_1);
      input.set_text_stream(true);
    }
  }


  void start()
  {
    while(!m_caffApp.shouldQuit())
    {
      const auto &data = m_laptopControl.get_active_buffer().get_data();
    
    
      m_caffApp.startFrame();

      const float delta_time = m_caffApp.getDeltaTime();
      
      // Update / Render
      {
        m_caffApp.getInput().set_mouse_hold(true);
        
        m_network.think(delta_time);
        m_laptop.think(delta_time);
        
        m_sceneController.update(delta_time, m_caffApp.getInput());

        CaffApp::Dev::FrameBuffer& console_output = m_laptop.render(m_caffApp.getRenderer());

        m_caffApp.getRenderer().clear();
        m_sceneView.draw(m_caffApp.getRenderer(), console_output);
      }

      m_terminal.think_systems(delta_time);

      m_caffApp.endFrame();
    }
  }


  void onTextStream(const std::string &str)
  {
    m_laptop.add_string_to_input(m_laptop.get_current_active_screen(), str);

    static std::string dataStr;
    dataStr = str;
  }


  void onKeyChange(const caff_app::key_id id, const caff_app::key_state state)
  {
    if(state == caff_app::key_state::UP)
    {
      if(id == caff_app::key_id::KB_BACKSPACE)
      {
        m_laptop.backspace_input(m_laptop.get_current_active_screen());
      }

      if(id == caff_app::key_id::KB_ENTER || id == caff_app::key_id::KB_RETURN)
      {
        const std::string input = m_laptop.get_input(m_laptop.get_current_active_screen());
        
        m_laptop.add_string_to_screen(m_laptop.get_current_active_screen(), input);

        m_laptop.clear_input(m_laptop.get_current_active_screen());
      }
    }
  }

private:

  caffeine::application::application     m_caffApp;

  SceneModel                             m_sceneModel;
  SceneView                              m_sceneView;
  SceneController                        m_sceneController;
  
  console_screen_controller              m_laptop;
  Laptop                                 m_laptopControl;
  Network                                m_network;
  TerminalConnections                    m_terminal = TerminalConnections(m_laptop, 4);
  

};



int main(int argc, char **argv)
{
  // Logging
  CaffUtil::SetLogLevel(CaffUtil::LogLevel::ERROR_LOG | CaffUtil::LogLevel::INFO_LOG | CaffUtil::LogLevel::WARNING_LOG);
  CaffUtil::SetOutputLogTargets(CaffUtil::LogOutput::CONSOLE);

  application app;
  app.start();

  return 0;
}