
#include <Application/terminal/system_factory.hpp>
#include <Application/terminal/programs/boot_sequence.hpp>
#include <Application/terminal/programs/dict_hack.hpp>
#include <assert.h>

namespace system_factory
{

std::vector<detail::terminal_program_interface>
create(const enum system id)
{
  std::vector<detail::terminal_program_interface> programs;
  
  switch(id)
  {
    case(system::NONE):
      programs.reserve(1);
      programs.emplace_back(boot_sequence());
      break;
  
  
  
    default:
      assert(false);
  };
  
  return programs;
}

} // namespace