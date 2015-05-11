#include <Application/terminal/programs/basic_system.hpp>
#include <Application/terminal/terminal_screen.hpp>


basic_system::basic_system(const uint32_t options)
: m_options(options)
, m_connection_msg("")
, m_disconnection_msg("")
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
    const std::vector<uint32_t> content(std::begin(self.connection_msg()), std::end(self.connection_msg()));
    controller.add_line_to_screen(content);
  }
}