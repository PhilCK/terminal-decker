#ifndef BASIC_SYSTEM_INCLUDED_80D5A361_C361_441B_8948_E35FC591A675
#define BASIC_SYSTEM_INCLUDED_80D5A361_C361_441B_8948_E35FC591A675


#include <stdint.h>


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

  explicit            basic_system(const uint32_t options = sys_opts::CLEAR | sys_opts::LS | sys_opts::NET);
  
  void                connect();
  void                think();
  void                disconnect();
  
private:

  const uint32_t      m_options = 0;

}; // class


void on_connection(basic_system &self);


#endif // include guard