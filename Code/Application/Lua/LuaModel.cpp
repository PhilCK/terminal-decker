
#include <Application/Lua/LuaModel.hpp>
#include <Application/Console/TextConsoleController.hpp>
#include <Caffeine/Common/Utilities/directories.hpp>
#include <lualib.h>
#include <iostream>






namespace
{
  using ReturnVals = int;
  TextConsoleController *termConsoleController = nullptr;
  CaffApp::Application *caffApplication = nullptr;

  ReturnVals terminal_clear(lua_State *L)
  {
    if(termConsoleController)
    {
      termConsoleController->clearScreen();
    }

    return 0;
  }

  ReturnVals terminal_ls(lua_State *L)
  {
    return 0;
  }

  ReturnVals terminal_conn(lua_State *L)
  {
    return 0;
  }
  
  ReturnVals terminal_number_of_local_instances(lua_State *L)
  {
    return 2;
  }

  ReturnVals terminal_echo(lua_State *L)
  {
    if(termConsoleController)
    {
      const std::string str = lua_tostring(L, 1);

      termConsoleController->addStringToBuffer(str);
    }

    return 0;
  }

  ReturnVals terminal_quit(lua_State *L)
  {
    if(caffApplication)
    {
      caffApplication->requestQuit();
    }

    return 0;
  }

  const luaL_Reg terminal_funcs[] =
  {
      { "clear",    terminal_clear  },
      { "ls",       terminal_ls     },
      { "connect",  terminal_conn   },
      { "echo",     terminal_echo   },
      { "quit",     terminal_quit   },
      { NULL,       NULL            },
  };
}


LuaModel::LuaModel(TextConsoleController &consoleController, CaffApp::Application &caffApp)
: L(nullptr)
, m_consoleController(consoleController)
, m_caffApp(caffApp)
{
  termConsoleController = &m_consoleController;
  caffApplication = &m_caffApp;

  L = luaL_newstate();
  luaL_openlibs(L);

  lua_newtable(L);
  lua_setglobal(L, "terminal");

  //luaL_newmetatable(L, "terminald");
  lua_getglobal(L, "terminal");
  luaL_setfuncs(L, terminal_funcs, 0);
  
  const std::string package_path = "package.path = package.path .. ';" + CaffUtil::GetPathDir() + "GameCode/?.lua'";
  luaL_dostring(L, package_path.c_str());

  const std::string entry(CaffUtil::GetPathDir() + "GameCode/Terminal/main.lua");

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
  

  lua_toboolean(L, -1); // return bool
  lua_pop(L, 1);

}