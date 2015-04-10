
#include <modules/console/console_module.hpp>
#include <modules/common/data_type_id.hpp>
#include <core/data_core.hpp>


namespace
{
  
}


void
on_register(ConsoleModule &self, const std::size_t id)
{
  self.module_id = id;
}


void
on_start(ConsoleModule &self)
{
  caffcore::request_notifications_of_data_nodes(self.module_id, TypeID::TEXT_INPUT);
}


void
on_think(ConsoleModule &self)
{
  
}


void
on_data_change(ConsoleModule &self, const std::vector<caffcore::DataNode> &nodes)
{
  const char* a = static_cast<char*>(nodes[0].data);

  
}
