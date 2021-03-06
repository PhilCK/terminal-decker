#ifndef BasicSystem_INCLUDED_80D5A361_C361_441B_8948_E35FC591A675
#define BasicSystem_INCLUDED_80D5A361_C361_441B_8948_E35FC591A675


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


class BasicSystem
{
public:

  explicit            BasicSystem(const std::string &connect_msg,
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


void on_connection(TerminalController &controller, BasicSystem &self);
void on_think(TerminalController &controller, BasicSystem &self);


#endif // include guard