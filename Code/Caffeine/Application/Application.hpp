

#ifndef CAFFEINE_APPLICATION_APPLICATION_INCLUDED
#define CAFFEINE_APPLICATION_APPLICATION_INCLUDED

#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>
#include <Caffeine/Common/Utilities/Timer.hpp>
#include <Caffeine/Common/Utilities/NonCopyable.hpp>
#include <Caffeine/Common/Utilities/Param.hpp>
#include <string>
#include <memory>


namespace Caffeine {
namespace Application {


/*
	Application
	-----------
	Main application class.

 */

class Application final : private CaffUtil::NonCopyable
{
public:
    
  explicit            Application(const std::string &title, const uint32_t width, const uint32_t height, const bool isFullscreen);
                      ~Application();
	
	void                startFrame();
  void                endFrame();
	
  inline float        getDeltaTime() const  { return m_deltaTime;  }
  inline bool         shouldQuit() const    { return m_shouldQuit; }
	
  Dev::Device &       getRenderer() const;
  Window &            getWindow() const;
  Input &             getInput() const;

	
private:

	std::unique_ptr<Window>		    m_window;
	std::unique_ptr<Dev::Device>	m_renderer;
	std::unique_ptr<Input>		    m_input;
	Utilities::Timer              m_gameTimer;
	float                         m_deltaTime   = 0.f;
	bool                          m_shouldQuit  = false;
	
}; // class


} // namespace
} // namespace

#endif // include guard
