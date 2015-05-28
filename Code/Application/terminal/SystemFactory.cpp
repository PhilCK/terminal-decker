#include <Application/Terminal/SystemFactory.hpp>
#include <Application/Terminal/Programs/BootSequence.hpp>
#include <Application/Terminal/Programs/DictHack.hpp>
#include <Application/Terminal/Programs/BasicSystem.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <assert.h>


namespace system_factory {


std::vector<detail::TerminalProgramInterface>
create(const enum SystemID id)
{
  std::vector<detail::TerminalProgramInterface> programs;
  
  switch(id)
  {
    case(SystemID::NONE):
      programs.reserve(1);
      programs.emplace_back(BootSequence());
      break;
  
    case(SystemID::LOCAL_HOST):
      programs.reserve(1);
      programs.emplace_back(BasicSystem("Connected to localhost:", ""));
      break;
      
    case(SystemID::RADIO):
    
      break;
      
    case(SystemID::DIGITAL_ADVERT_AGENCY):
    
      break;
      
    case(SystemID::RAIL_ADVERT_SYS):
    
      break;
  
  
    default:
      caff_util::log_error("Tried to build something that doens't exsit.");
      assert(false);
  };
  
  return programs;
}


} // namespace