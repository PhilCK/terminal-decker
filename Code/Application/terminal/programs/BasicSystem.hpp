#ifndef BASIC_SYSTEM_INCLUDED_80D5A361_C361_441B_8948_E35FC591A675
#define BASIC_SYSTEM_INCLUDED_80D5A361_C361_441B_8948_E35FC591A675


#include <Application/Terminal/TerminalFwd.hpp>
#include <stdint.h>
#include <string>


namespace sys_opts {


enum ENUM
{
  PENDING_LOGIN   = 1 << 0,
  CLEAR           = 1 << 1,
  LS              = 1 << 2,
  NET             = 1 << 3,
}; // enum


} // namespace


class basic_system
{
public:

  explicit            basic_system(const std::string &connect_msg,
                                   const std::string &disconnect_msg,
                                   const uint32_t options = sys_opts::CLEAR | sys_opts::LS | sys_opts::NET);
  
  void                connect();
  void                think();
  void                disconnect();
  
  inline std::string  connection_msg() const    { return m_connection_msg;    }
  inline std::string  disconnection_msg() const { return m_disconnection_msg; }
  
private:

  const uint32_t      m_options = 0;
  const std::string   m_connection_msg = "connected to localhost";
  const std::string   m_disconnection_msg = "";

}; // class


void on_connection(terminal_controller &controller, basic_system &self);
void on_think(terminal_controller &controller, basic_system &self);


#endif // include guard