#ifndef LUA_MODEL_INCLUDED_D7BB0EBA_EE4F_4123_A8F9_9940C6CE76D
#define LUA_MODEL_INCLUDED_D7BB0EBA_EE4F_4123_A8F9_9940C6CE76D

#include <lua.hpp>
#include <string>
#include <Application/Console/TextConsoleFwd.hpp>
#include <Caffeine/Application/Application.hpp>

class LuaModel
{
public:

  explicit                LuaModel(TextConsoleController &consoleController, CaffApp::Application &caffApp);
                          ~LuaModel();

  void                    onLoaded();
  void                    onUpdate();
  void                    onCommand(const std::string &cmd, const std::string &args);

private:

  lua_State               *L = nullptr;
  TextConsoleController   &m_consoleController;
  CaffApp::Application    &m_caffApp;

};

#endif // include guard