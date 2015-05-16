#ifndef APPLICATION_APPLICATION_INCLUDED_E276BA0E_C4CB_46E0_99BF_4858FF5A35FA
#define APPLICATION_APPLICATION_INCLUDED_E276BA0E_C4CB_46E0_99BF_4858FF5A35FA


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>
#include <Caffeine/Common/Utilities/Timer.hpp>
#include <Caffeine/Common/Utilities/Param.hpp>
#include <string>
#include <memory>


namespace caffeine {
namespace application {


class application final
{

	application(const application &) = delete;
	application& operator=(const application&) = delete;

public:
    
  explicit                    application(const std::string &title, const uint32_t width, const uint32_t height, const bool isFullscreen);
                              ~application();
	
	void                        startFrame();
  void                        endFrame();
	
  inline float                getDeltaTime() const  { return m_deltaTime;  }
  inline bool                 shouldQuit() const    { return m_shouldQuit; }
  inline void                 requestQuit()         { m_shouldQuit = true; }
	
  CaffApp::Dev::Device &      getRenderer() const;
  CaffApp::Window &           getWindow() const;
  caff_app::input &           getInput() const;

	
private:

	std::unique_ptr<CaffApp::Window>        m_window;
	std::unique_ptr<CaffApp::Dev::Device>   m_renderer;
	std::unique_ptr<caff_app::input>        m_input;
	Caffeine::Utilities::Timer              m_gameTimer;
	float                                   m_deltaTime   = 0.f;
	bool                                    m_shouldQuit  = false;
	
}; // class


} // namespace
} // namespace


#endif // include guard