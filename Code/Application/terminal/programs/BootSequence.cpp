#include <Application/Terminal/Programs/BootSequence.hpp>
#include <Application/Terminal/TerminalScreen.hpp>
#include <Application/Terminal/SystemFactory.hpp>


BootSequence::BootSequence()
{
  
}


std::vector<uint32_t>
BootSequence::start_boot()
{
  if(m_current_boot_state != boot_state::OFF)
  {
    return std::vector<uint32_t>(); // already booting.
  }

  m_current_boot_state = boot_state::START;
  
  const std::string boot = "powering up ...\n";
  std::vector<uint32_t> content(std::begin(boot), std::end(boot));
  return content;
}


std::vector<uint32_t>
BootSequence::update_boot(const float dt)
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
      
      const std::string boot = ".";
      std::vector<uint32_t> content(std::begin(boot), std::end(boot));
      //return content;
    
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


void
on_connection(TerminalController& controller, BootSequence &self)
{
  const auto content = self.start_boot();
  
  controller.add_line_to_screen(content);
}


void
on_disconnection(TerminalController& controller, BootSequence &self)
{
  controller.clear_screen();
}


void
on_think(TerminalController& controller, BootSequence &self, const float dt)
{
  const auto content = self.update_boot(dt);
  
  if(!content.empty())
  {
    controller.add_line_to_screen(content);
  }
  
  if(self.is_done())
  {
    controller.connect(SystemID::LOCAL_HOST);
  }
}