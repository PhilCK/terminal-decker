#ifndef CONSOLE_MODULE_INCLUDED_8FA8AB09_56B6_4F45_B0AF_7C31E3E4B1B5
#define CONSOLE_MODULE_INCLUDED_8FA8AB09_56B6_4F45_B0AF_7C31E3E4B1B5


#include <core/core.hpp>


class ConsoleModule
{
public:

  friend void       on_register(ConsoleModule &self, const std::size_t id);
  friend void       on_start(ConsoleModule &self);
  friend void       on_think(ConsoleModule &self);
  friend void       on_data_change(ConsoleModule &self, const std::vector<caffcore::DataNode> &nodes);

  CORE_INTERFACE_STUB(ConsoleModule, new_data)

private:

  std::size_t module_id = 0;

};



#endif // include guard