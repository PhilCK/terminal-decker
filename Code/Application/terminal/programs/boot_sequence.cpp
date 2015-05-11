#include <Application/terminal/programs/boot_sequence.hpp>
#include <Application/terminal/terminal_screen.hpp>
#include <Application/terminal/system_factory.hpp>


boot_sequence::boot_sequence()
{
  
}


std::vector<uint32_t> boot_sequence::start_boot()
{
  if(m_current_boot_state != boot_state::OFF)
  {
    return std::vector<uint32_t>(); // already booting.
  }

  m_current_boot_state = boot_state::START;
  
  const std::string boot = "powering up ...";
  std::vector<uint32_t> content(std::begin(boot), std::end(boot));
  return content;
}


std::vector<uint32_t> boot_sequence::update_boot(const float dt)
{
  m_boot_timer += dt;

  switch(m_current_boot_state)
  {
    case(boot_state::START):
    {
      if(m_boot_timer > 1)
      {
        m_current_boot_state = boot_state::CHECKING_VOL;
      }
    
      break;
    }
    case(boot_state::CHECKING_VOL):
    {
      if(m_boot_timer > 2)
      {
        const std::string msg = "Checking volumes ... [Done]";
        const std::vector<uint32_t> content(std::begin(msg), std::end(msg));
        
        m_current_boot_state = boot_state::FINISHED;
        
        return content;
      }
      
      break;
    }
    case(boot_state::FINISHED):
    {
      // Request delete, or signal ready for system change to localhost.
      break;
    }
    
    case(boot_state::OFF):
    default:
    break;
  }
  
  return std::vector<uint32_t>();
}


void on_connection(terminal_controller& controller, boot_sequence &self)
{
  const auto content = self.start_boot();
  
  controller.add_line_to_screen(content);
}


void on_think(terminal_controller& controller, boot_sequence &self, const float dt)
{
  const auto content = self.update_boot(dt);
  
  if(!content.empty())
  {
    controller.add_line_to_screen(content);
  }
  
  if(self.is_done())
  {
    controller.connect(system_id::LOCAL_HOST);
  }
}