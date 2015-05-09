
#include <Application/terminal/system_factory.hpp>
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
    case(system::local_host):
      programs.reserve(5);
      programs.emplace_back(dictionary_hack());
      break;
  
      
    case(system::elevator_control):
      break;
  
  
    default:
      assert(false);
  };
  
  return programs;
}

} // namespace