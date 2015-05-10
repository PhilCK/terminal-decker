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
#include <Caffeine/Application/Input.hpp>

#include <Application/Console/TextConsoleModel.hpp>
#include <Application/Console/TextConsoleView.hpp>
#include <Application/Console/TextDataParse.hpp>

#include <Application/Scene/SceneController.hpp>
#include <Application/Scene/SceneView.hpp>
#include <Application/Scene/SceneModel.hpp>

#include <vector>

#include <core/core.hpp>
#include <core/data_core.hpp>
#include <modules/console/console_module.hpp>

#include <Application/Console/console_screen_controller.hpp>

#include <Application/terminal/terminal.hpp>

#include <SDL_mixer.h>
#include <SDL2/SDL.h>

namespace
{
  const uint32_t width = 1280;
  const uint32_t height = 720;
  // 1280 × 720
  // 864 x 486
  
  Mix_Chunk *sound1 = nullptr;
  Mix_Chunk *sound2 = nullptr;
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

    // Audio
    {
      if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
      {
          printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
      }
      
      const std::string path1 = CaffUtil::GetPathDir() + "Audio/test_1.wav";
       sound1 = Mix_LoadWAV(path1.c_str());
      
             const std::string path2 = CaffUtil::GetPathDir() + "Audio/test_2.wav";
       sound2 = Mix_LoadWAV(path2.c_str());
      
       Mix_SetPosition(0, 270, 100);
       Mix_PlayChannel(0, sound1, 0 );
      
      Mix_SetPosition(1, 270, 100);
       Mix_PlayChannel(1, sound2, 0 );

    }

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
    
      input.onKeyChangeEvent  = std::bind(&application::onKeyChange, this, std::placeholders::_1, std::placeholders::_2);
      input.onTextStreamEvent = std::bind(&application::onTextStream, this, std::placeholders::_1);
      input.setTextStream(true);
    }
  }


  void start()
  {
    while(!m_caffApp.shouldQuit())
    {
      m_caffApp.startFrame();

      const float delta_time = m_caffApp.getDeltaTime();
      
      // Update / Render
      {
        m_caffApp.getInput().setMouseHold(true);

        m_laptop.think(delta_time);
        
        m_sceneController.update(delta_time, m_caffApp.getInput());

        CaffApp::Dev::FrameBuffer& console_output = m_laptop.render(m_caffApp.getRenderer());

        m_caffApp.getRenderer().clear();
        m_sceneView.draw(m_caffApp.getRenderer(), console_output);
      }

      m_terminal.think_systems(delta_time);
  
      caffcore::think_all_modules();
      caffcore::dispatch_data_notifications();

      m_caffApp.endFrame();
    }
  }


  void onTextStream(const std::string &str)
  {
    m_laptop.add_string_to_input(m_laptop.get_current_active_screen(), str);

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
        m_laptop.backspace_input(m_laptop.get_current_active_screen());
      }

      if(id == CaffApp::KeyID::KB_ENTER || id == CaffApp::KeyID::KB_RETURN)
      {
        const std::string input = m_laptop.get_input(m_laptop.get_current_active_screen());
        
        m_laptop.add_string_to_screen(m_laptop.get_current_active_screen(), input);

        m_laptop.clear_input(m_laptop.get_current_active_screen());
      }
    }
  }

private:

  CaffApp::Application                    m_caffApp;

  SceneModel                              m_sceneModel;
  SceneView                               m_sceneView;
  SceneController                         m_sceneController;
  
  console_screen_controller               m_laptop;
  terminal                                m_terminal = terminal(4);
  

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

  application app;
  app.start();

  return 0;
}