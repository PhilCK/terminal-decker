#include <Application/terminal/terminal.hpp>
#include <Application/terminal/terminal_screen.hpp>
#include <Application/terminal/system_factory.hpp>
#include <Application/terminal/programs/dict_hack.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>


terminal::terminal(const uint32_t max_number_of_systems)
{
  m_systems.reserve(max_number_of_systems);

  for(uint32_t i = 0; i < max_number_of_systems; ++i)
  {
    CaffUtil::LogInfo("terminal - Creating inital laptop systems.");
    m_systems.emplace_back(system_factory::create(system::NONE));
  }
  
  for(auto &d : m_systems)
  {
    for(auto &p : d)
    {
      terminal_screen screen(0);
      on_connection(screen, p);
    }
  }
}


void terminal::think_systems(const float dt)
{
  for(auto &d : m_systems)
  {
    for(auto &p : d)
    {
      terminal_screen screen(0);
      on_think(screen, p, dt);
    }
  }
}


void terminal::input_string(const std::string &str)
{
  for(auto &d : m_systems)
  {
    for(auto &p : d)
    {
      terminal_screen screen(0);
      on_input_str(screen, p, str);
    }
  }
}