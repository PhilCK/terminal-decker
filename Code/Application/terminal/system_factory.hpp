#ifndef SYSTEM_FACTORY_INCLUDED_C58A846A_683F_417F_852F_874A66D81017
#define SYSTEM_FACTORY_INCLUDED_C58A846A_683F_417F_852F_874A66D81017


#include <Application/terminal/terminal.hpp>
#include <Application/terminal/terminal_module.hpp>
#include <vector>
#include <stdint.h>


enum class system_id : uint8_t
{
  NONE,
  LOCAL_HOST,
};


namespace system_factory
{

std::vector<detail::terminal_program_interface> create(const enum system_id id);

} // namespace


#endif // include guard