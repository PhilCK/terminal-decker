#ifndef TERMINAL_INCLUDED_5C68C166_D5E8_4359_8678_4F59ED0CBCFD
#define TERMINAL_INCLUDED_5C68C166_D5E8_4359_8678_4F59ED0CBCFD


#include <Application/terminal/terminal_module.hpp>
#include <string>
#include <stdint.h>
#include <vector>


class terminal
{
public:

  
  explicit              terminal(const uint32_t max_number_of_systems);

  void                  think_systems(const float dt);
  void                  input_string(const std::string &str);
  
  
private:

  std::vector<std::vector<detail::terminal_program_interface>>      m_systems;

}; // class

#endif // include guard