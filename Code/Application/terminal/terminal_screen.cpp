#include <Application/terminal/terminal_screen.hpp>
#include <iostream>


terminal_screen::terminal_screen(const uint32_t screen_id)
: m_screen_id(screen_id)
{
}


void
terminal_screen::add_line_to_screen(const std::string &str)
{
  std::cout << str << std::endl;
}


void
terminal_screen::add_line_to_screen(const std::vector<uint32_t> &content)
{
  std::cout << "content" << std::endl;
}


void
terminal_screen::blit_screen()
{
  std::cout << "blit" << std::endl;
}