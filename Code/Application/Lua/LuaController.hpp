#ifndef LUA_CONTROLLER_INCLUDED_3EEB73C5_1C91_4777_949F_559074A124D9
#define LUA_CONTROLLER_INCLUDED_3EEB73C5_1C91_4777_949F_559074A124D9

#include <Application/Lua/LuaFwd.hpp>

class LuaController
{
public:

  explicit            LuaController(LuaModel &model);

  void                updateCallback();
  void                commandCallback();

private:

  LuaModel            &m_model;

};

#endif // include guard