
#include <Application/terminal/terminal.hpp>
#include <Application/terminal/system_factory.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <Application/terminal/programs/dict_hack.hpp>


terminal::terminal(const uint32_t max_number_of_systems)
{
  m_systems.reserve(max_number_of_systems);

  for(uint32_t i = 0; i < max_number_of_systems; ++i)
  {
    CaffUtil::LogInfo("terminal - Creating inital laptop systems.");
    m_systems.emplace_back(system_factory::create(system::local_host));
  }
  
  for(auto &d : m_systems)
  {
    for(auto &p : d)
    {
      on_connection(p);
    }
  }
}


void terminal::think_systems(const float dt)
{
  for(auto &d : m_systems)
  {
    for(auto &p : d)
    {
      on_think(p, dt);
    }
  }
}


void terminal::input_string(const std::string &str)
{
  // Trim args, loop and pass
  // terminal_program_input(input_cmd, input_args);
}