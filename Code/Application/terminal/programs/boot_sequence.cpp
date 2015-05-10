#include <Application/terminal/programs/boot_sequence.hpp>
#include <Application/terminal/terminal_screen.hpp>


boot_sequence::boot_sequence()
{
  
}


std::vector<uint32_t> boot_sequence::start_boot()
{
  m_current_boot_state = boot_state::START;
  
  return std::vector<uint32_t>();
}


std::vector<uint32_t> boot_sequence::update_boot(const float dt)
{
  switch(m_current_boot_state)
  {
    case(boot_state::START):
    {
      break;
    }
    case(boot_state::RUNNING):
    {
      break;
    }
    case(boot_state::FINISHED):
    {
      break;
    }
    
    case(boot_state::OFF):
    default:
    break;
  }
  
  return std::vector<uint32_t>();
}


void on_connection(terminal_screen& screen, boot_sequence &self)
{
  self.start_boot();
}


void on_think(terminal_screen& screen, boot_sequence &self, const float dt)
{
  
}