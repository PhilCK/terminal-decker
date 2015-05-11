#ifndef TERMINAL_CONTROLLER_INCLUDED_C56929BE_7E4C_4B04_A74A_365061C39DC9
#define TERMINAL_CONTROLLER_INCLUDED_C56929BE_7E4C_4B04_A74A_365061C39DC9


#include <Application/Console/console_screen_controller.hpp>
//#include <Application/terminal/system_factory.hpp>
#include <Application/terminal/terminal_fwd.hpp>
#include <vector>
#include <string>
#include <stdint.h>


class terminal_controller
{
public:

  explicit            terminal_controller(console_screen_controller &console_controller, terminal_connections &connections, const uint32_t screen_id);
  
  void                add_line_to_screen(const std::vector<uint32_t> &content);
  void                blit_screen();
  
  void                connect(const enum system_id connect_sys);
  
private:

  console_screen_controller &m_screen_controller;
  terminal_connections      &m_connections;
  const uint32_t            m_screen_id = 0;

}; // class


#endif // include guard