#include <Application/terminal/programs/basic_system.hpp>
#include <Application/terminal/terminal_screen.hpp>


basic_system::basic_system(const std::string &connect_msg,
                           const std::string &disconnect_msg,
                           const uint32_t options)
: m_options(options)
, m_connection_msg(connect_msg)
, m_disconnection_msg(disconnect_msg)
{
}


void
basic_system::think()
{
}


void
on_connection(terminal_controller& controller, basic_system &self)
{
  if(!self.connection_msg().empty())
  {
    const std::string msg = self.connection_msg();
    const std::vector<uint32_t> content(std::begin(msg), std::end(msg));
    
    controller.add_line_to_screen(content);
  }
}