#ifndef SYSTEM_FACTORY_INCLUDED_C58A846A_683F_417F_852F_874A66D81017
#define SYSTEM_FACTORY_INCLUDED_C58A846A_683F_417F_852F_874A66D81017


#include <Application/Terminal/Terminal.hpp>
#include <Application/Terminal/TerminalModule.hpp>
#include <vector>
#include <stdint.h>


enum class SystemID : uint8_t
{
  NONE,
  LOCAL_HOST,
  RADIO,
  RAIL_ADVERT_SYS,
  DIGITAL_ADVERT_AGENCY,
};


namespace system_factory
{

std::vector<detail::TerminalProgramInterface> create(const enum SystemID id);

} // namespace


#endif // include guard