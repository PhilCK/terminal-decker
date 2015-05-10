#include <Caffeine/Application/input.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <string>


namespace caffeine {
namespace application {


input::input()
{
  set_text_stream(false); // SDL seems to have this on by default.
}


input::~input()
{
}


uint32_t input::get_mouse_x() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	return x;
}


uint32_t input::get_mouse_y() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	return y;
}


void input::set_mouse_hold(const bool set)
{
	SDL_SetRelativeMouseMode(set ? SDL_TRUE : SDL_FALSE);
}


void input::set_text_stream(const bool set)
{
	set ? SDL_StartTextInput() : SDL_StopTextInput();
}


std::string input::get_key_name(const key_id id) const
{
	const std::string name = SDL_GetKeyName((int)id);
	return name;
}


namespace
{
  namespace caff = caffeine::application;

  inline bool check_state(const std::map<caff::key_id, caff::key_state> &states, const key_state check_state, const key_id id)
  {
  	auto it = states.find(id);
    
    if(it != states.end())
    {
      if(it->second == check_state)
      {
        return true;
      }
    }
    
    return false;
  }
}


bool input::is_down(const key_id id) const
{
  return check_state(m_key_states, key_state::DOWN, id);
}


bool input::is_down_on_frame(const key_id id) const
{
  return check_state(m_frame_key_states, key_state::DOWN, id);
}


bool input::is_up_on_frame(const key_id id) const
{
  return check_state(m_frame_key_states, key_state::UP, id);
}


void input::think()
{
	m_frame_key_states.clear();
  m_x_delta = m_y_delta = 0;
}


void input::force_clear()
{
  m_frame_key_states.clear();
  m_key_states.clear();
  m_x_delta = m_y_delta = 0;
}


void input::pump_system_event(const void *data)
{
  // Return if you sent me nothing.
  if (!data)
  {
    CaffUtil::LogError("Input: No data passed to event pump.");
    return;
  }
  
  auto update_key = [&](const key_id id, const key_state state)
  {
    m_key_states[id]       = state;
    m_frame_key_states[id] = state;
    
    if (on_key_change_event)
    {
      on_key_change_event(id, state);
    }
  };

  const SDL_Event *sdl_event = static_cast<const SDL_Event*>(data);

  switch (sdl_event->type)
  {
    case(SDL_KEYDOWN):
    {
      const SDL_Keycode key = sdl_event->key.keysym.sym;
      update_key((key_id)key, key_state::DOWN);

      if(key == SDLK_PASTE)
      {
        std::cout << "Paste!" << std::endl;
      }

      break;
    }
    
    case(SDL_KEYUP):
    {
      const SDL_Keycode key = sdl_event->key.keysym.sym;
      update_key((key_id)key, key_state::UP);

      break;
    }

    case(SDL_TEXTINPUT):
    {
      if (on_text_stream_event)
      {
        const std::string text = sdl_event->text.text;
        on_text_stream_event(text);
      }

      break;
    }

    case(SDLK_PASTE):
    case(SDL_TEXTINPUTEVENT_TEXT_SIZE):
    case(SDL_TEXTEDITING):
    case(SDL_SCANCODE_PASTE):
    {
      break;
    }

    case(SDL_MOUSEMOTION):
    {
      m_x_delta = static_cast<float>(sdl_event->motion.xrel);
      m_y_delta = static_cast<float>(sdl_event->motion.yrel);

      if (on_mouse_delta_event)
      {
        on_mouse_delta_event(m_x_delta, m_y_delta);
      }

      if (on_mouse_move_position_event)
      {
        on_mouse_move_position_event(static_cast<float>(get_mouse_x()), static_cast<float>(get_mouse_y()));
      }

      break;
    }

    case(SDL_MOUSEBUTTONDOWN) :
    {
      switch (sdl_event->button.button)
      {
      case(SDL_BUTTON_LEFT) :
        update_key(key_id::MS_LEFT, key_state::DOWN);
        break;

      case(SDL_BUTTON_MIDDLE) :
        update_key(key_id::MS_CENTER, key_state::DOWN);
        break;

      case(SDL_BUTTON_RIGHT) :
        update_key(key_id::MS_RIGHT, key_state::DOWN);
        break;
      }

      break;
    }

    case(SDL_MOUSEBUTTONUP) :
    {
      switch (sdl_event->button.button)
      {
        case(SDL_BUTTON_LEFT):
          update_key(key_id::MS_LEFT, key_state::UP);
          break;

        case(SDL_BUTTON_MIDDLE):
          update_key(key_id::MS_CENTER, key_state::UP);
          break;

        case(SDL_BUTTON_RIGHT):
          update_key(key_id::MS_RIGHT, key_state::UP);
          break;
      }

      break;
    }
  }
}


} // namespace
} // namespace