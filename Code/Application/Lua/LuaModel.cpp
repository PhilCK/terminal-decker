
#include <Application/Lua/LuaModel.hpp>
#include <Caffeine/Common/Utilities/directories.hpp>
#include <lualib.h>
#include <iostream>


namespace
{
  using ReturnVals = int;

  ReturnVals terminal_clear(lua_State *L)
  {
    std::cout << "terminal clear" << std::endl;
    return 0;
  }

  const luaL_Reg terminal_funcs[] =
  {
      { "clear", terminal_clear},
      { NULL, NULL }
  };
}


LuaModel::LuaModel()
{
  if(this == NULL)
  {
    std::cout << "MMoop" << std::endl;
  }

  L = luaL_newstate();
  luaL_openlibs(L);

  lua_newtable(L);
  lua_setglobal(L, "terminal");

  //luaL_newmetatable(L, "terminald");
  lua_getglobal(L, "terminal");
  luaL_setfuncs(L, terminal_funcs, 0);

  const std::string entry(CaffUtil::GetPathDir() + "GameCode\\Terminal\\main.lua");

  if(luaL_dofile(L, entry.c_str()))
  {
      printf("%s\n", lua_tostring(L, -1));
  }



   //lua_pushcfunction(L, terminal_clear);
   //lua_setglobal(L, "terminal.clear");
}


LuaModel::~LuaModel()
{
  lua_close(L);
}


void LuaModel::onLoaded()
{
  luaL_dostring(L, "terminal.load()");
}


void LuaModel::onUpdate()
{
  //luaL_dostring(L, "terminal.update()");

  lua_getglobal(L, "update");
  //lua_pushnumber(L, 5);
  lua_pcall(L, 0, 0, 0);
  //cout << "The return value of the function was " << lua_tostring(L, -1) << endl;
  //lua_pop(L,1);

  lua_getglobal(L, "terminal");
  lua_getfield(L, -1, "update");
  lua_pcall(L, 0, 0, 0);

}


void LuaModel::onCommand(const std::string &command, const std::string &args)
{
  lua_getglobal(L, "terminal");
  lua_getfield(L, -1, "input_string");
  lua_pushstring(L, command.c_str());
  if(lua_pcall(L, 1, 1, 0))
  {
    std::cout << "Lua error: " << luaL_checkstring(L, -1) << std::endl;
  }
  

  bool retValue = lua_toboolean(L, -1);
  lua_pop(L, 1);

}