#include <Application/Terminal/Programs/BasicSystem.hpp>
#include <Application/Terminal/TerminalScreen.hpp>


BasicSystem::BasicSystem(const std::string &connect_msg,
                           const std::string &disconnect_msg,
                           const uint32_t options)
: m_options(options)
, m_connection_msg(connect_msg)
, m_disconnection_msg(disconnect_msg)
{
}


void
BasicSystem::think()
{
}


void
on_connection(TerminalController& controller, BasicSystem &self)
{
  if(!self.connection_msg().empty())
  {
    const std::string msg = self.connection_msg();
    const std::vector<uint32_t> content(std::begin(msg), std::end(msg));
    
    controller.add_line_to_screen(content);
  }
}


void
on_think(TerminalController &controller, BasicSystem &self)
{
  
  
  
}