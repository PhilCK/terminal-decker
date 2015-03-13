

#include <Caffeine/Application/Application.hpp>
#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Renderer/Device.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Common/Utilities/Timer.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>


namespace Caffeine {
namespace Application {


Application::Application(const std::string & title, const uint32_t width, const uint32_t height, const bool isFullscreen)
: m_window(nullptr)
, m_renderer(nullptr)
, m_input(nullptr)
, m_gameTimer()
, m_deltaTime(0.f)
, m_shouldQuit(false)
{
  // Init SDL.
	const Uint32 flags = SDL_INIT_EVERYTHING;

	if(SDL_Init(flags) != 0)
	{
		throw std::runtime_error(SDL_GetError()); // TODO: Errr!
	}
	
	// SDL Attribute Hints.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	
	// Create Input, Window and Renderer.
	m_window.reset(new Window(title, width, height, isFullscreen));
	m_renderer.reset(new Dev::Device(m_window->getSDLWindow()));
	m_input.reset(new Input());
}


Application::~Application()
{
	m_renderer.release();
	m_window.release();

	SDL_ERROR("Application::~Application");

	SDL_Quit();
}


void Application::startFrame()
{
	m_deltaTime = m_gameTimer.split() * 0.001f;
	
  // Input
  {
    m_input->update();
    m_window->update();
  }

  // SDL Event Checking
  {
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
      // Check for quit message.
      if (sdlEvent.type == SDL_QUIT)
      {
        m_shouldQuit = true;
      }

      m_input->pumpSystemEvent(&sdlEvent);
      m_window->pumpSystemEvent(&sdlEvent);
    }
  }
}


void Application::endFrame()
{
	// Swap buffer.
	SDL_GL_SwapWindow(m_window->getSDLWindow());
	SDL_ERROR("Application::endFrame()");

}


Dev::Device & Application::getRenderer() const
{
  assert(m_renderer);
	return *m_renderer;
}


Window & Application::getWindow() const
{
  assert(m_window);
	return *m_window;
}


Input & Application::getInput() const
{
  assert(m_input);
	return *m_input;
}


} // namespace
} // namespace
