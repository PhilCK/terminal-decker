#ifndef BootSequence_INCLUDED_B73D3BB0_2183_4DF9_9E9B_F67A37A10CDD
#define BootSequence_INCLUDED_B73D3BB0_2183_4DF9_9E9B_F67A37A10CDD


#include <Application/Terminal/TerminalFwd.hpp>
#include <stdint.h>
#include <vector>


class BootSequence
{
public:

  explicit                  BootSequence();
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


void on_connection(TerminalController& controller, BootSequence &self);
void on_disconnection(TerminalController& controller, BootSequence &self);
void on_think(TerminalController& controller, BootSequence &self, const float dt);


#endif // include guard