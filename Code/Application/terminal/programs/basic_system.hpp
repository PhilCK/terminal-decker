#ifndef BASIC_SYSTEM_INCLUDED_80D5A361_C361_441B_8948_E35FC591A675
#define BASIC_SYSTEM_INCLUDED_80D5A361_C361_441B_8948_E35FC591A675


class basic_system
{
public:

  explicit            basic_system();
  
  void                connect();
  void                think();
  void                disconnect();

}; // class


void on_connection(basic_system &self);


#endif // include guard