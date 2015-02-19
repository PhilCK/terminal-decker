

#ifndef CAFFEINE_APPLICATION_WINDOW_INCLUDED
#define CAFFEINE_APPLICATION_WINDOW_INCLUDED


#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/NonCopyable.hpp>
#include <string>
#include <memory>
#include <functional>


namespace Caffeine {
namespace Application {

 
class Window : private CaffUtil::NonCopyable
{
public:
    
  explicit               Window(const std::string & title, const uint32_t width, const uint32_t height, const bool isFullscreen);
                        ~Window();
    

  bool                  setResolution(const uint32_t width, const uint32_t height);
  uint32_t              getWidth() const;
  uint32_t              getHeight() const;
	

  bool                  setFullscreen(bool const isFullscreen);
	inline bool           isFullscreen() const { return m_isFullscreen; }
	

  bool                  setTitle(const std::string & setTitle);
  std::string           getTitle() const;
	

  inline SDL_Window*    getSDLWindow() const { return m_sdlWindow; }


  // ** EVENTS ** //
  
  std::function<void()>       onLostFocusEvent;
  std::function<void()>       onGotFocusEvent;
  std::function<void()>       onMinimizedEvent;
  std::function<void()>       onMaximizedEvent;
  std::function<void()>       onResizeEvent;
  std::function<void()>       onWindowMovedEvent;
  std::function<void()>       onWindowClosedEvent;


private:


  friend class CaffApp::Application;

  void                  update() {}
  void                  pumpSystemEvent(const void * sysEventData);
	

private:
	
  bool					        m_isFullscreen;
  SDL_Window            *m_sdlWindow;

}; // class
    
} // namespace
} // namespace

#endif // include guard
