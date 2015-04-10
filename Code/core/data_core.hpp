#ifndef DATA_CORE_INCLUDED_
#define DATA_CORE_INCLUDED_


#include <array>


namespace caffeine {
namespace core {


struct DataNode
{
	unsigned int type_id;
	unsigned int instance_id;

  void *data;
  std::size_t bytes;
  std::size_t numberOfElements;

	std::array<unsigned int, 16> attribute_ids;
};



unsigned int 			add_data_to_core(DataNode node);

bool					register_data_type(const std::size_t i);
void 					request_notifications_of_data_nodes(const std::size_t module_id, const std::size_t type_id);
void 					dispatch_data_notifications();


} // namespace
} // namespace

#endif // include guard