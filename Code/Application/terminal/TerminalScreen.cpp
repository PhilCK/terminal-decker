#include <Application/Terminal/TerminalScreen.hpp>
#include <Application/Terminal/SystemFactory.hpp>


TerminalController::TerminalController(console_screen_controller &console_controller, TerminalConnections &connections, const uint32_t screen_id)
: m_screen_controller(console_controller)
, m_connections(connections)
, m_screen_id(screen_id)
{
}


void
TerminalController::add_line_to_screen(const std::vector<uint32_t> &content)
{
  m_screen_controller.add_content_to_screen(m_screen_id, content);
}


void
TerminalController::blit_screen()
{

}


void
TerminalController::clear_screen()
{
  m_screen_controller.clear_screen(m_screen_id);
}


void
TerminalController::connect(const enum SystemID connect_sys)
{
  m_connections.update_connection(m_screen_id, connect_sys);
}