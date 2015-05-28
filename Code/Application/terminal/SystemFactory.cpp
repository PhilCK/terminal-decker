#include <Application/Terminal/SystemFactory.hpp>
#include <Application/Terminal/Programs/BootSequence.hpp>
#include <Application/Terminal/Programs/DictHack.hpp>
#include <Application/Terminal/Programs/BasicSystem.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
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
      programs.emplace_back(basic_system("Connected to localhost:", ""));
      break;
      
    case(system_id::RADIO):
    
      break;
      
    case(system_id::DIGITAL_ADVERT_AGENCY):
    
      break;
      
    case(system_id::RAIL_ADVERT_SYS):
    
      break;
  
  
    default:
      caff_util::log_error("Tried to build something that doens't exsit.");
      assert(false);
  };
  
  return programs;
}


} // namespace