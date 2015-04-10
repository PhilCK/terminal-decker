#include <core/core.hpp>
#include <vector>
#include <assert.h>


namespace
{
	std::vector<caffeine::core::CoreModuleRegisterDetails> core_modules_info;
	std::vector<caffeine::core::detail::CoreModule> core_modules;


	inline std::size_t to_index(const std::size_t i) 	{ return i - 1; }
	inline std::size_t to_id(const std::size_t i) 		{ return i + 1; }
}


namespace caffeine {
namespace core {


std::size_t
push_new_module(detail::CoreModule new_module)
{
	core_modules.emplace_back(new_module);
	core_modules_info.emplace_back(CoreModuleRegisterDetails{""});

	return core_modules.size();
}


void
register_module_details(const std::size_t id, const CoreModuleRegisterDetails &details)
{
	core_modules_info.at(to_index(id)).name = details.name;
}


void
push_new_core_state() {}

void
pop_core_state() {}


void
force_data_update(unsigned int instance_id)
{

}


void
send_data_to_module(const std::size_t module_id, const std::vector<DataNode> &nodes)
{
	assert(module_id <= core_modules.size());
	on_data_change(core_modules.at(to_index(module_id)), nodes);
}


// Engine hooks
void
register_all_modules()
{
	for(std::size_t i = 0; i < core_modules.size(); ++i)
	{
		on_register(core_modules.at(i), to_id(i));
	}
}


void
start_all_modules()
{
	for(auto &module : core_modules)
	{
		on_start(module);
	}
}


void
think_all_modules()
{
	for(auto &module : core_modules)
	{
		on_think(module);
	}
}

	
} // namespace
} // namespace