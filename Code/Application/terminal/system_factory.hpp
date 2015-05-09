#ifndef SYSTEM_FACTORY_INCLUDED_C58A846A_683F_417F_852F_874A66D81017
#define SYSTEM_FACTORY_INCLUDED_C58A846A_683F_417F_852F_874A66D81017


#include <Application/terminal/terminal.hpp>
#include <vector>


enum class system
{
  local_host,
  elevator_control,
};


namespace system_factory
{

std::vector<detail::terminal_program_interface> create(const enum system system_id);

} // namespace


#endif // include guard