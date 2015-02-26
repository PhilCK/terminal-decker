
#include <Application/Console/TextConsoleController.hpp>
#include <Application/Console/TextConsoleModel.hpp>

TextConsoleController::TextConsoleController(TextConsoleModel &model)
: m_model(model)
{
}


void TextConsoleController::addStringToBuffer(const std::string &str)
{
  m_model.addStringToBuffer(str);
}


void TextConsoleController::addStringToInput(const std::string &str)
{
  m_model.addStringToInput(str);
}