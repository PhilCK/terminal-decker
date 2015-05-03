/*
  Lua Interface
  --
  This provides the api that is used inside lua.
*/


#ifndef LUA_INTERFACE_INCLUDED_918FBC01_C9A1_4BAE_B919_FD299C0EEC9A
#define LUA_INTERFACE_INCLUDED_918FBC01_C9A1_4BAE_B919_FD299C0EEC9A


#include <Application/Lua/LuaFwd.hpp>
#include <Application/Console/console_screen_controller.hpp>
#include <Caffeine/Application/Application.hpp>
#include <lua.hpp>
#include <string>


namespace lua_interface {

lua_State* get_state();

// Create the lua_State* and does some prep.
void create_lua_state();

// Pass the access into models that Lua needs to access in its functions.
void register_modules_available_in_lua(console_screen_controller *consoleController, CaffApp::Application *caffApp);

// Registers native access in lua.
void register_lua_functions();

// Call main lua file.
void call_main_lua_file(const std::string &filepath = "");

// terminal.on_loaded() when the game starts up.
void call_lua_on_loaded();

// terminal.on_update() a game tick.
void call_lua_on_update(const float dt);

// terminal.on_command(input_str) when a command has been entered into the terminal.
void call_lua_on_command(const std::string &input);

// terminal.on_quit() when the user or game has init a quit request.
void call_lua_on_quit();


} // namespace


#endif // include guard