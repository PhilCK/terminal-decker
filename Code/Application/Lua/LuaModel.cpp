
#include <Application/Lua/LuaModel.hpp>
#include <iostream>


LuaModel::LuaModel()
{
  if(this == NULL)
  {
    std::cout << "MMoop" << std::endl;
  }

  L = luaL_newstate();
  luaL_openlibs(L);

  const std::string entry("C:\\Users\\Gunhead\\Desktop\\TerminalGame\\Assets\\GameCode\\Terminal\\main.lua");

  if(luaL_dofile(L, entry.c_str()))
  {
      printf("%s\n", lua_tostring(L, -1));
  }


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
  lua_pcall(L, 1, 1, 0);
  
  bool retValue = lua_toboolean(L, -1);
  lua_pop(L, 1);

}