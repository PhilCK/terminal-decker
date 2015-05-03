
#include <Application/Console/console_screen_controller.hpp>
#include <assert.h>


namespace
{
  const uint32_t width  = 80;
  const uint32_t height = 24;
  const auto font_data  = FontData::ParseData("none");
}


console_screen_controller::console_screen_controller(const uint32_t number_of_screens)
: m_number_of_screens(number_of_screens)
, m_view(width, height, 4, font_data.maxWidth, font_data.maxHeight)
{
  for(uint32_t i = 0; i < number_of_screens; ++i)
  {
    m_screen_models.emplace_back(new TextConsoleModel(width, height, font_data));
  }
}


void console_screen_controller::clear_screen(const uint32_t screen_id)
{
    m_screen_models.at(screen_id)->clearBuffer();
}


void console_screen_controller::add_string_to_screen(const uint32_t screen_id, const std::string &str)
{
  std::vector<uint32_t> content(str.begin(), str.end());
  add_content_to_screen(screen_id, content);
}


void console_screen_controller::add_content_to_screen(const uint32_t screen_id, const std::vector<uint32_t> &content)
{
  m_screen_models.at(screen_id)->addStringToBuffer(content);
}


std::string console_screen_controller::get_input(const uint32_t screen_id)
{
  return m_screen_models.at(screen_id)->getInput();
}


void console_screen_controller::add_string_to_input(const uint32_t screen_id, const std::string &str)
{
  m_screen_models.at(screen_id)->addStringToInput(str);
}


void console_screen_controller::backspace_input(const uint32_t screen_id)
{
  m_screen_models.at(screen_id)->backspaceInput();
}


void console_screen_controller::clear_input(const uint32_t screen_id)
{
  m_screen_models.at(screen_id)->clearInput();
}


void console_screen_controller::set_prompt(const uint32_t screen_id, const std::string &new_prompt)
{
  m_screen_models.at(screen_id)->setInputPrompt(new_prompt);
}


void console_screen_controller::broadcast_msg(const std::vector<uint32_t> on_screens)
{
  
}


void console_screen_controller::think(const float dt)
{
  for(auto &model : m_screen_models)
  {
    model->prepareData();
  }
}


CaffApp::Dev::FrameBuffer & console_screen_controller::render(CaffApp::Dev::Device &renderer)
{
  return m_view.render(*m_screen_models.at(m_view_screen), renderer);
}