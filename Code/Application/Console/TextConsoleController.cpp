
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


void TextConsoleController::addInputToBuffer()
{
  m_model.addStringToBuffer(m_model.getInput());
  m_model.clearInput();
}


void TextConsoleController::backspaceInput()
{
  m_model.backspaceInput();
}


void TextConsoleController::clearInput()
{
  m_model.clearInput();
}


void TextConsoleController::setPrompt(const std::string &str)
{
  m_model.setInputPrompt(str);
}