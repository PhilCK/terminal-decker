#include <Application/terminal/terminal_screen.hpp>
#include <Application/terminal/system_factory.hpp>


terminal_controller::terminal_controller(console_screen_controller &console_controller, terminal_connections &connections, const uint32_t screen_id)
: m_screen_controller(console_controller)
, m_connections(connections)
, m_screen_id(screen_id)
{
}


void
terminal_controller::add_line_to_screen(const std::vector<uint32_t> &content)
{
  m_screen_controller.add_content_to_screen(m_screen_id, content);
}


void
terminal_controller::blit_screen()
{

}


void
terminal_controller::clear_screen()
{
  m_screen_controller.clear_screen(m_screen_id);
}


void
terminal_controller::connect(const enum system_id connect_sys)
{
  m_connections.update_connection(m_screen_id, connect_sys);
}