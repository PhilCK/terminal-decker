#ifndef TEXT_CONSOLE_CONTROLLER_INCLUDED_303B1F18_46EC_4647_BC88_87F36BD35638
#define TEXT_CONSOLE_CONTROLLER_INCLUDED_303B1F18_46EC_4647_BC88_87F36BD35638

#include <Application/Console/TextConsoleFwd.hpp>
#include <Caffeine/Common/Platform.hpp>
#include <string>
#include <vector>

class TextConsoleController
{
public:

  explicit            TextConsoleController(TextConsoleModel &model);
                      ~TextConsoleController() {}

  void                addStringToBuffer(const std::string &str);
  void                addContentToBuffer(const std::vector<uint32_t> &content);

  void                addStringToInput(const std::string &str);
  void                addInputToBuffer();
  void                backspaceInput();
  void                clearInput();
  void                clearScreen();
  void                setPrompt(const std::string &str);

private:

  TextConsoleModel    &m_model;

}; // class

#endif // include guard