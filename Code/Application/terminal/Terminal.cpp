#include <Application/Terminal/Terminal.hpp>
#include <Application/Terminal/TerminalScreen.hpp>
#include <Application/Terminal/SystemFactory.hpp>
#include <Application/Terminal/Programs/DictHack.hpp>
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
    
    auto &id = m_pending_connections.at(s);
    
    if(id != system_id::NONE)
    {
      // Disconnect
      for(uint32_t i = 0; i < static_cast<uint32_t>(sys.size()); ++i)
      {
        terminal_controller controller(m_screen_controller, *this, s);
        on_disconnection(controller, sys.at(i));
      }
    
      sys = system_factory::create(id);
      id = system_id::NONE;
      
      // Connect to new system
      for(uint32_t i = 0; i < static_cast<uint32_t>(sys.size()); ++i)
      {
        terminal_controller controller(m_screen_controller, *this, s);
        on_connection(controller, sys.at(i));
      }
    }
  }
}


void
terminal_connections::input_string(const std::string &str)
{
  for(uint32_t s = 0; s < m_systems.size(); ++s)
  {
    auto &sys = m_systems.at(s);
    const uint32_t size_of_systems = static_cast<uint32_t>(sys.size());
    terminal_controller controller(m_screen_controller, *this, s);
    
    // TODO: Looping on to virtual boo!
    for(uint32_t i = 0; i < size_of_systems; ++i)
    {
      on_input_str(controller, sys.at(i), str);
    }
    
    auto &id = m_pending_connections.at(s);
    
    if(id != system_id::NONE)
    {
      // Disconnect
      for(uint32_t i = 0; i < size_of_systems; ++i)
      {
        on_disconnection(controller, sys.at(i));
      }
    
      sys = system_factory::create(id);
      id = system_id::NONE;
      
      // Connect to new system
      for(uint32_t i = 0; i < size_of_systems; ++i)
      {
        on_connection(controller, sys.at(i));
      }
    }
  }
}


void
terminal_connections::update_connection(const uint32_t screen, enum system_id id)
{
  m_pending_connections.at(screen) = id;
}