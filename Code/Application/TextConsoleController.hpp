#ifndef TEXT_CONSOLE_CONTROLLER_INCLUDED_303B1F18_46EC_4647_BC88_87F36BD35638
#define TEXT_CONSOLE_CONTROLLER_INCLUDED_303B1F18_46EC_4647_BC88_87F36BD35638

class TextConsoleModel;

class TextConsoleController
{
public:

  explicit            TextConsoleController();
                      ~TextConsoleController();

  void                addStringToBuffer();
  void                addStringToInput();

private:

    TextConsoleModel  &m_model;

}; // class

#endif // include guard