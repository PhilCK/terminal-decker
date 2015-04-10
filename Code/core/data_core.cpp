#include <core/data_core.hpp>
#include <core/core.hpp>
#include <vector>


namespace
{
	std::vector<caffeine::core::DataNode> data_nodes;
	std::vector<std::pair<std::size_t , std::size_t>> data_notifications;
	std::vector<std::size_t> register_data_types;
}


namespace caffeine {
namespace core {


unsigned int
add_data_to_core(DataNode node)
{	
	data_nodes.size();

	data_nodes.emplace_back(node);
	data_nodes.back().instance_id = data_nodes.size();

	return data_nodes.back().instance_id;
}


bool
register_data_type(const std::size_t type_id)
{
	const auto it = std::find(register_data_types.begin(), register_data_types.end(), type_id);

	return it != register_data_types.end() ? true : false;
}


void
request_notifications_of_data_nodes(const std::size_t module_id, const std::size_t type_id)
{
	data_notifications.push_back(std::pair<std::size_t, std::size_t>(type_id, module_id));
}


void
dispatch_data_notifications()
{
	for(const auto &notification : data_notifications)
	{
    if(!data_nodes.empty())
    {
		  send_data_to_module(notification.second, data_nodes);
    }
	}

  data_nodes.clear();
}

} // namespace
} // namespace