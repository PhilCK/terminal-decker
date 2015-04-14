
#include <Application/Console/TextConsoleController.hpp>
#include <Application/Console/TextConsoleModel.hpp>

TextConsoleController::TextConsoleController(TextConsoleModel &model)
: m_model(model)
{
}


void TextConsoleController::addStringToBuffer(const std::string &str)
{
  std::vector<uint32_t> content(str.begin(), str.end());

  m_model.addStringToBuffer(content);
}


void TextConsoleController::addContentToBuffer(const std::vector<uint32_t> &content)
{
  m_model.addStringToBuffer(content);
}


void TextConsoleController::addStringToInput(const std::string &str)
{
  m_model.addStringToInput(str);
}


void TextConsoleController::addInputToBuffer()
{
  addStringToBuffer(m_model.getInput());
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


void TextConsoleController::clearScreen()
{
  m_model.clearBuffer();
}


void TextConsoleController::setPrompt(const std::string &str)
{
  m_model.setInputPrompt(str);
}