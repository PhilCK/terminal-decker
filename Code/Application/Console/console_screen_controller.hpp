#ifndef CONSOLE_SCREENS_INCLUDED_184E18E3_9B61_43A6_AD42_872D427F6BC1
#define CONSOLE_SCREENS_INCLUDED_184E18E3_9B61_43A6_AD42_872D427F6BC1


#include <Application/Console/TextConsoleModel.hpp>
#include <Application/Console/TextConsoleView.hpp>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>


class console_screen_controller
{

  console_screen_controller(const console_screen_controller&) = delete;
  console_screen_controller& operator=(const console_screen_controller&) = delete;

public:

  explicit            console_screen_controller(const uint32_t number_of_screens);
  
  // ** General Interaction ** //
  inline void set_active_screen(const uint32_t active_screen) {
    assert(m_number_of_screens > active_screen);
    m_view_screen = active_screen;
  }
  
  inline uint32_t     get_number_of_screens() const { return m_number_of_screens; }
  inline uint32_t     get_current_active_screen() const { return m_view_screen; }
  
  
  // ** Screen interaction ** //
  void                clear_screen(const uint32_t screen_id);
  void                add_string_to_screen(const uint32_t screen_id, const std::string &str);
  void                add_content_to_screen(const uint32_t screen_id, const std::vector<uint32_t> &content);
  
  
  // ** Input interaction ** //
  std::string         get_input(const uint32_t screen_id);
  void                add_string_to_input(const uint32_t screen_id, const std::string &str);
  void                backspace_input(const uint32_t screen_id);
  void                clear_input(const uint32_t screen_id);
  void                set_prompt(const uint32_t screen_id, const std::string &new_prompt);
  
  
  // ** Interaction with other screens ** //)
  void                broadcast_msg(const std::vector<uint32_t> on_screens);
  
  
  // ** Other ** //
  void                          think(const float dt);
  CaffApp::Dev::FrameBuffer &   render(CaffApp::Dev::Device &renderer);
  
  
private:

  uint32_t            m_number_of_screens = 0;
  uint32_t            m_view_screen = 0;
  std::vector<std::unique_ptr<TextConsoleModel> > m_screen_models;
  text_console_view     m_view;

}; // class


#endif // include guard