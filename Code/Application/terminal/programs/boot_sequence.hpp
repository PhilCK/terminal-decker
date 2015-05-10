#ifndef BOOT_SEQUENCE_INCLUDED_B73D3BB0_2183_4DF9_9E9B_F67A37A10CDD
#define BOOT_SEQUENCE_INCLUDED_B73D3BB0_2183_4DF9_9E9B_F67A37A10CDD


#include <Application/terminal/terminal_fwd.hpp>
#include <stdint.h>
#include <vector>


class boot_sequence
{
public:

  explicit                  boot_sequence();
  std::vector<uint32_t>     start_boot();
  std::vector<uint32_t>     update_boot(const float dt);
  
private:

  enum class boot_state
  {
    OFF,
    START,
    RUNNING,
    FINISHED,
  };
  
  boot_state                m_current_boot_state = boot_state::OFF;
  uint32_t                  m_boot_timer = 0;

}; // class


void on_connection(terminal_screen& screen, boot_sequence &self);
void on_think(terminal_screen& screen, boot_sequence &self, const float dt);


#endif // include guard