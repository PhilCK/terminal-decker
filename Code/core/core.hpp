#ifndef CORE_INCLUDED_
#define CORE_INCLUDED_


#include <core/detail/core_module.hpp>
#include <core/module_details.hpp>
#include <vector>
#include <string>


namespace caffcore = caffeine::core;


namespace caffeine {
namespace core {


std::size_t		push_new_module(detail::CoreModule newModule);
void 			register_module_details(const std::size_t id, const CoreModuleRegisterDetails &details);

void			push_new_core_state();
void			pop_core_state();
void			force_data_update(unsigned int instance_id);

void 			send_data_to_module(const std::size_t module_id, const std::vector<DataNode> &nodes);
void 			register_module_details();

// Module hooks

void 			register_all_modules();
void 			start_all_modules();
void 			think_all_modules();
void 			data_node_notification();



} // namespace
} // namespace


#endif // include guard