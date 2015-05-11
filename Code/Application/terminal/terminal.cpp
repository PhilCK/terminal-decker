#include <Application/terminal/terminal.hpp>
#include <Application/terminal/terminal_screen.hpp>
#include <Application/terminal/system_factory.hpp>
#include <Application/terminal/programs/dict_hack.hpp>
#include <Application/Console/console_screen_controller.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>


terminal_connections::terminal_connections(console_screen_controller &screen_controller, const uint32_t max_number_of_systems)
: m_screen_controller(screen_controller)
, m_pending_connections(max_number_of_systems, system_id::NONE)
{
  m_systems.reserve(max_number_of_systems);

  for(uint32_t i = 0; i < max_number_of_systems; ++i)
  {
    CaffUtil::LogInfo("terminal - Creating inital laptop systems.");
    m_systems.emplace_back(system_factory::create(system_id::NONE));
  }
  
  for(uint32_t s = 0; s < m_systems.size(); ++s)
  {
    auto &sys = m_systems.at(s);
  
    for(uint32_t i = 0; i < static_cast<uint32_t>(sys.size()); ++i)
    {
      terminal_controller controller(m_screen_controller, *this, s);
      on_connection(controller, sys.at(i));
    }
  }
}


void
terminal_connections::think_systems(const float dt)
{
  for(uint32_t s = 0; s < m_systems.size(); ++s)
  {
    auto &sys = m_systems.at(s);
    
    for(uint32_t i = 0; i < static_cast<uint32_t>(sys.size()); ++i)
    {
      terminal_controller controller(m_screen_controller, *this, s);
      on_think(controller, sys.at(i), dt);
    }
    
    const auto id = m_pending_connections.at(s);
    
    if(id != system_id::NONE)
    {
      sys = system_factory::create(id);
    }
  }
}


void
terminal_connections::input_string(const std::string &str)
{
  for(uint32_t s = 0; s < m_systems.size(); ++s)
  {
    auto &sys = m_systems.at(s);
    
    for(uint32_t i = 0; i < static_cast<uint32_t>(sys.size()); ++i)
    {
      terminal_controller controller(m_screen_controller, *this, s);
      on_input_str(controller, sys.at(i), str);
    }
    
    const auto id = m_pending_connections.at(s);
    
    if(id != system_id::NONE)
    {
      sys = system_factory::create(id);
    }
  }
}


void
terminal_connections::update_connection(const uint32_t screen, enum system_id id)
{
  m_pending_connections.at(screen) = id;
}