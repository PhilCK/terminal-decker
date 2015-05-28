#ifndef BOOT_SEQUENCE_INCLUDED_B73D3BB0_2183_4DF9_9E9B_F67A37A10CDD
#define BOOT_SEQUENCE_INCLUDED_B73D3BB0_2183_4DF9_9E9B_F67A37A10CDD


#include <Application/Terminal/TerminalFwd.hpp>
#include <stdint.h>
#include <vector>


class boot_sequence
{
public:

  explicit                  boot_sequence();
  std::vector<uint32_t>     start_boot();
  std::vector<uint32_t>     update_boot(const float dt);
  inline bool               is_done() const { return m_current_boot_state == boot_state::FINISHED; }
  
private:

  enum class boot_state
  {
    OFF,
    START,
    CHECKING_VOL,
    FINISHED,
  };
  
  boot_state                m_current_boot_state = boot_state::OFF;
  float                     m_boot_timer = 0;

}; // class


void on_connection(terminal_controller& controller, boot_sequence &self);
void on_disconnection(terminal_controller& controller, boot_sequence &self);
void on_think(terminal_controller& controller, boot_sequence &self, const float dt);


#endif // include guard