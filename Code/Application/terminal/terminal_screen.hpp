#ifndef TERMINAL_SCREEN_INCLUDED_C56929BE_7E4C_4B04_A74A_365061C39DC9
#define TERMINAL_SCREEN_INCLUDED_C56929BE_7E4C_4B04_A74A_365061C39DC9


#include <vector>
#include <stdint.h>
#include <string>


class terminal_screen
{
public:

  explicit            terminal_screen(const uint32_t screen_id);
  
  void                add_line_to_screen(const std::string &str);
  void                add_line_to_screen(const std::vector<uint32_t> &content);
  void                blit_screen();
  
private:

  uint32_t            m_screen_id = 0;

}; // class


#endif // include guard