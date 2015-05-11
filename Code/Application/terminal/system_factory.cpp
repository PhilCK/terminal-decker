#include <Application/terminal/system_factory.hpp>
#include <Application/terminal/programs/boot_sequence.hpp>
#include <Application/terminal/programs/dict_hack.hpp>
#include <Application/terminal/programs/basic_system.hpp>
#include <assert.h>


namespace system_factory {


std::vector<detail::terminal_program_interface>
create(const enum system_id id)
{
  std::vector<detail::terminal_program_interface> programs;
  
  switch(id)
  {
    case(system_id::NONE):
      programs.reserve(1);
      programs.emplace_back(boot_sequence());
      break;
  
    case(system_id::LOCAL_HOST):
      programs.reserve(1);
      programs.emplace_back(basic_system());
      break;
  
  
    default:
      assert(false);
  };
  
  return programs;
}


} // namespace