

#include <Caffeine/Application/Input.hpp>
#include <string>
#include <iostream>


namespace Caffeine {
namespace Application {


Input::Input()
: m_keystates()
, m_frameKeystates()
, m_xDelta(0.f)
, m_yDelta(0.f)
{
  setTextStream(false); // SDL seems to have this on by default.
}


Input::~Input()
{
}


uint32_t Input::getMouseX() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	return x;
}


uint32_t Input::getMouseY() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	return y;
}


void Input::setMouseHold(const bool isHeld)
{
	SDL_SetRelativeMouseMode(isHeld ? SDL_TRUE : SDL_FALSE);
}


void Input::setTextStream(const bool setTextStream)
{
	setTextStream ? SDL_StartTextInput() : SDL_StopTextInput();
}


std::string Input::getKeyName(const KeyID keyID) const
{
	const std::string name = SDL_GetKeyName((int)keyID);
	return name;
}


bool Input::isDown(const KeyID keyID) const
{
	auto it = m_keystates.find(keyID);
	
	if(it != m_keystates.end())
	{
		if(it->second == KeyState::DOWN) {
			return true;
		}
	}
	
	return false;
}


bool Input::isDownOnFrame(const KeyID keyID) const
{
	auto it = m_frameKeystates.find(keyID);
	
	if(it != m_frameKeystates.end())
	{
		if(it->second == KeyState::DOWN)
		{
			return true;
		}
	}
	
	return false;
}


bool Input::isUpOnFrame(const KeyID keyID) const
{
	auto it = m_frameKeystates.find(keyID);

	if (it != m_frameKeystates.end())
	{
		if (it->second == KeyState::UP)
		{
			return true;
		}
	}

	return false;
}


void Input::update()
{
	m_frameKeystates.clear();
  m_xDelta = m_yDelta = 0;
}


void Input::clear()
{
  m_frameKeystates.clear();
  m_keystates.clear();
}


void Input::pumpSystemEvent(const void * sysEventData)
{
  // Return if you sent me nothing.
  if (!sysEventData)
  {
    return;
  }
  
  auto updateKey = [&](const KeyID key, const KeyState state)
  {
    m_keystates[key]      = state;
    m_frameKeystates[key] = state;
    
    if (onKeyChangeEvent)
    {
      onKeyChangeEvent(key, state);
    }
  };

  const SDL_Event *sdlEvent = static_cast<const SDL_Event*>(sysEventData);

  switch (sdlEvent->type)
  {
    case(SDL_KEYDOWN):
    {
      const SDL_Keycode key = sdlEvent->key.keysym.sym;
      updateKey((KeyID)key, KeyState::DOWN);

      break;
    }
    
    case(SDL_KEYUP):
    {
      const SDL_Keycode key = sdlEvent->key.keysym.sym;
      updateKey((KeyID)key, KeyState::UP);

      break;
    }

    case(SDL_TEXTINPUT):
    {
      if (onTextStreamEvent)
      {
        const std::string streamTxt = sdlEvent->text.text;
        onTextStreamEvent(streamTxt);
      }

      break;
    }

    case(SDL_MOUSEMOTION):
    {
      m_xDelta = static_cast<float>(sdlEvent->motion.xrel);
      m_yDelta = static_cast<float>(sdlEvent->motion.yrel);

      if (onMouseDeltaEvent)
      {
        onMouseDeltaEvent(m_xDelta, m_yDelta);
      }

      if (onMouseMovePositionEvent)
      {
        onMouseMovePositionEvent(static_cast<float>(getMouseX()), static_cast<float>(getMouseY()));
      }

      break;
    }

    case(SDL_MOUSEBUTTONDOWN) :
    {
      switch (sdlEvent->button.button)
      {
      case(SDL_BUTTON_LEFT) :
        updateKey(CaffApp::KeyID::MS_LEFT, KeyState::DOWN);
        break;

      case(SDL_BUTTON_MIDDLE) :
        updateKey(CaffApp::KeyID::MS_CENTER, KeyState::DOWN);
        break;

      case(SDL_BUTTON_RIGHT) :
        updateKey(CaffApp::KeyID::MS_RIGHT, KeyState::DOWN);
        break;
      }

      break;
    }

    case(SDL_MOUSEBUTTONUP) :
    {
      switch (sdlEvent->button.button)
      {
        case(SDL_BUTTON_LEFT):
          updateKey(CaffApp::KeyID::MS_LEFT, KeyState::UP);
          break;

        case(SDL_BUTTON_MIDDLE):
          updateKey(CaffApp::KeyID::MS_CENTER, KeyState::UP);
          break;

        case(SDL_BUTTON_RIGHT):
          updateKey(CaffApp::KeyID::MS_RIGHT, KeyState::UP);
          break;
      }

      break;
    }
  }
}


} // namespace
} // namespace
