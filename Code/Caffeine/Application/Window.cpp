

#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>


namespace Caffeine {
namespace Application {


Window::Window(const std::string & title, const uint32_t width, const uint32_t height, const bool isFullscreen)
: m_sdlWindow(nullptr)
, m_isFullscreen(isFullscreen)
{
	// Setup the SDL Window.
  Uint32 flags = m_isFullscreen ? SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_OPENGL;

  m_sdlWindow = SDL_CreateWindow(title.c_str(),
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								width,
								height,
								flags);

	SDL_ERROR("Window::Window()");
}


Window::~Window()
{
	SDL_DestroyWindow(m_sdlWindow);
	SDL_ERROR("Window::~Window()");
}


bool Window::setResolution(const uint32_t width, const uint32_t height)
{
  int32_t currWidth, currHight;
  SDL_GetWindowSize(m_sdlWindow, &currWidth, &currHight);
  SDL_ERROR("Window::setResolution() - 1");

  if ((currWidth == width) && (currHight == height))
  {
    return false;
  }

  SDL_SetWindowSize(m_sdlWindow, static_cast<int32_t>(width), static_cast<int32_t>(height));
  SDL_ERROR("Window::setResolution() - 2");
  return true;
}


uint32_t Window::getWidth() const
{
	int w, h;
	
	SDL_GetWindowSize(m_sdlWindow, &w, &h);
	SDL_ERROR("Window::getWidth()");
    
  return static_cast<int32_t>(w);
}


uint32_t Window::getHeight() const
{
	int w, h;
	
	SDL_GetWindowSize(m_sdlWindow, &w, &h);
	SDL_ERROR("Window::getHeight()");

  return static_cast<int32_t>(h);
}


bool Window::setFullscreen(const bool isFullscreen)
{
	m_isFullscreen = isFullscreen;
	
  SDL_SetWindowFullscreen(m_sdlWindow, m_isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	SDL_ERROR("Window::setFullscreen()");

	return true;
}


bool Window::setTitle(const std::string & setTitle)
{
  SDL_SetWindowTitle(m_sdlWindow, setTitle.c_str());
	SDL_ERROR("Window::setTitle()");
	
	return true;
}


std::string Window::getTitle() const
{
  const std::string title = SDL_GetWindowTitle(m_sdlWindow);
  SDL_ERROR("Window::getTitle()");

  return title;
}


void Window::pumpSystemEvent(const void * sysEventData)
{
  // Return if you sent me nothing.
  if (!sysEventData)
  {
    return;
  }

  const SDL_Event *sdlEvent = static_cast<const SDL_Event*>(sysEventData);

  if (sdlEvent->type == SDL_WINDOWEVENT)
  {
    switch (sdlEvent->window.event)
    {
      case(SDL_WINDOWEVENT_SHOWN): // When shown for the first time.
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_HIDDEN):
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_EXPOSED): // Called from start and also when window comes back from minimized. 
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_MOVED):
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_SIZE_CHANGED):
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_RESIZED): // Called when size window dragged or max button pressed. and sometimes when SetWindowSize() is called.
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_MAXIMIZED):
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_MINIMIZED):
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_RESTORED): // When minimised and brought back onto the screen.
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_ENTER): // When mouse pointer hovers over. Even if not in focus.
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_LEAVE): // When mouse pointer leaves the window. Even if not in focus.
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_FOCUS_GAINED):
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_FOCUS_LOST):
      {
        int i = 0;
        break;
      }

      case(SDL_WINDOWEVENT_CLOSE): // When x hit.
      {
        int i = 0;
        break;
      }
    }
  }
}


} // namespace
} // namespace
