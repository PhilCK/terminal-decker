#ifndef TERMINAL_CONTROLLER_INCLUDED_C56929BE_7E4C_4B04_A74A_365061C39DC9
#define TERMINAL_CONTROLLER_INCLUDED_C56929BE_7E4C_4B04_A74A_365061C39DC9


#include <Application/Console/console_screen_controller.hpp>
#include <Application/Terminal/TerminalFwd.hpp>
#include <vector>
#include <string>
#include <stdint.h>


class TerminalController
{
public:

  explicit            TerminalController(console_screen_controller &console_controller,
                                          TerminalConnections &connections,
                                          const uint32_t screen_id);
  
  void                add_line_to_screen(const std::vector<uint32_t> &content);
  void                blit_screen();
  void                clear_screen();
  
  void                connect(const enum class SystemID connect_sys);
  
private:

  console_screen_controller &m_screen_controller;
  TerminalConnections      &m_connections;
  const uint32_t            m_screen_id = 0;

}; // class


#endif // include guard