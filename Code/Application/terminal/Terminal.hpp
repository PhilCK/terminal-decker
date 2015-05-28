#ifndef TERMINAL_INCLUDED_5C68C166_D5E8_4359_8678_4F59ED0CBCFD
#define TERMINAL_INCLUDED_5C68C166_D5E8_4359_8678_4F59ED0CBCFD


#include <Application/Terminal/TerminalModule.hpp>
#include <Application/Terminal/TerminalFwd.hpp>
#include <Application/Console/TextConsoleFwd.hpp>
#include <string>
#include <stdint.h>
#include <vector>


class terminal_connections
{
public:
  
  
  explicit              terminal_connections(
                          console_screen_controller &screen_controller,
                          const uint32_t max_number_of_systems);

  void                  think_systems(const float dt);
  void                  input_string(const std::string &str);
  void                  update_connection(const uint32_t screen, enum system_id id);
  
  
private:

  std::vector<std::vector<detail::terminal_program_interface>>m_systems;
  std::vector<enum system_id> m_pending_connections;
  console_screen_controller &m_screen_controller;

}; // class

#endif // include guard