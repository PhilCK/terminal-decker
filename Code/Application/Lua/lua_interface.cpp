
#include <Application/Lua/lua_interface.hpp>
#include <Caffeine/Common/Utilities/directories.hpp>
#include <Caffeine/Math/Math.hpp>
#include <assert.h>
#include <iostream>


namespace
{
  lua_State *L = nullptr;
  const char* api_table = "terminal";
  
  console_screen_controller *console_access = nullptr;
  CaffApp::Application *application_access  = nullptr;

  // Terminal API

  int echo_to_screen(lua_State *L)
  {
    if(console_access)
    {
      const uint32_t screen_id = lua_tonumber(L, 1);
      const std::string str = lua_tostring(L, 2);
      console_access->add_string_to_screen(screen_id - 1, str);
    }
  
    return 0;
  }

  int number_of_screens(lua_State *L)
  {
    if(console_access)
    {
      const uint32_t number_of_screens = console_access->get_number_of_screens();
      lua_pushnumber(L, number_of_screens);
    }
    
    return 1;
  }
  
  int get_current_screen(lua_State *L)
  {
    if(console_access)
    {
      const uint32_t current_screen = console_access->get_current_active_screen() + 1;
      lua_pushnumber(L, current_screen);
    }
    
    return 1;
  }

  int switch_active_screen(lua_State *L)
  {
    if(console_access)
    {
      const uint32_t next_screen = lua_tonumber(L, 1);
      const uint32_t clamped_id = CaffMath::Clamp<uint32_t>(next_screen, 1, console_access->get_number_of_screens());
      const uint32_t from_lua_index = clamped_id - 1;
      console_access->set_active_screen(from_lua_index);
    }
    
    return 0;
  }

  int clear_screen(lua_State *L)
  {
    if(console_access)
    {
      const uint32_t screen_id = lua_tonumber(L, 1);
      console_access->clear_screen(screen_id - 1);
    }
    return 0;
  }

  int quit_application(lua_State *L)
  {
    if(application_access)
    {
      application_access->requestQuit();
    }

    return 0;
  }
  
  int set_input_prompt(lua_State *L)
  {
    return 0;
  }
}


namespace lua_interface
{

lua_State* get_state()
{
  return L;
}


void register_lua_functions()
{
  // Register lua api.
  const luaL_Reg terminal_funcs[] =
  {
    { "clear",                  clear_screen          },
    { "get_number_of_screens",  number_of_screens     },
    { "get_current_screen_id",  get_current_screen    },
    { "set_active_screen",      switch_active_screen  },
    { "quit",                   quit_application      },
    { "echo",                   echo_to_screen        },
    { "set_prompt",             set_input_prompt      },
    { NULL,                     NULL                  },
  };
  
  lua_getglobal(L, api_table);
  luaL_setfuncs(L, terminal_funcs, 0);
}


void create_lua_state()
{
  // Create new lua state
  L = luaL_newstate();
  assert(L);
  
  luaL_openlibs(L);
  
  // Create a terminal table for callbacks.
  lua_newtable(L);
  lua_setglobal(L, api_table);
  
  // Set a package path for 'require' statements in lua.
  std::string bin_path = CaffUtil::GetPathDir();
  std::replace(std::begin(bin_path), std::end(bin_path), '\\', '/'); // Lua seems to want unix style brackets for this to work.

  const std::string package_path = std::string("package.path = package.path .. ';") + bin_path + std::string("GameCode/?.lua'");
  luaL_dostring(L, package_path.c_str());
}


void register_modules_available_in_lua(console_screen_controller *console_controller, CaffApp::Application *caff_app)
{
  console_access = console_controller;
  application_access = caff_app;
}


void call_main_lua_file(const std::string &filepath /* = ""*/)
{
  std::string entry = filepath;

  if(entry.empty())
  {
    entry = CaffUtil::GetPathDir() + "GameCode/Terminal/main.lua";
  }
  
  // Lua entry point.
  if(luaL_dofile(L, entry.c_str()))
  {
      printf("%s\n", lua_tostring(L, -1));
  }
}


void call_lua_on_loaded()
{
  lua_getglobal(L, api_table);
  lua_getfield(L, -1, "on_load");
  
  if(lua_pcall(L, 0, 0, 0))
  {
    std::cout << "Lua Error on_load: " << luaL_checkstring(L, -1) << std::endl;
  }
}


void call_lua_on_update(const float delta_time)
{
  lua_getglobal(L, api_table);
  lua_getfield(L, -1, "on_think");
  lua_pushnumber(L, delta_time);
  
  if(lua_pcall(L, 1, 0, 0))
  {
    std::cout << "Lua Error on_think: " << luaL_checkstring(L, -1) << std::endl;
  }
}


void call_lua_on_command(const std::string &input)
{
  assert(console_access);

  lua_getglobal(L, api_table);
  lua_getfield(L, -1, "on_input_string");
  lua_pushstring(L, input.c_str());  
  lua_pushnumber(L, console_access->get_current_active_screen() + 1);
  
  if(lua_pcall(L, 2, 0, 0))
  {
    std::cout << "Lua Error on_input_string: " << luaL_checkstring(L, -1) << std::endl;
  }
}


void call_lua_on_quit()
{
  lua_getglobal(L, api_table);
  lua_getfield(L, -1, "on_quit");
  
  if(lua_pcall(L, 0, 0, 0))
  {
    std::cout << "Lua Error on_quit: " << luaL_checkstring(L, -1) << std::endl;
  }
}


} // namespace