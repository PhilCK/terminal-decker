
#include <Caffeine/Application/Renderer/Device.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>


namespace Caffeine {
namespace Application {
namespace Dev {


Device::Device(SDL_Window *sdlWindow)
{
  // Setup SDL Context.
	{
		m_glContext = SDL_GL_CreateContext(sdlWindow);
		SDL_ERROR("Renderer::Renderer() - SDL_GL_CreateContext");
		assert(m_glContext);
		
		SDL_GL_MakeCurrent(sdlWindow, m_glContext);
		SDL_ERROR("Renderer::Renderer() - SDL_GL_MakeCurrent");
	}
	
	// Output the version strings to be sure.	
	const std::string glContext = "OpenGL Context: " + std::string((char *)glGetString(GL_VERSION));
	const std::string glsl	    = "GLSL: " + std::string((char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	CaffUtil::LogInfo(glContext);
	CaffUtil::LogInfo(glsl);
  
	#ifdef CAFF_ON_WINDOWS
	// Initialise Glew
	{
		glewExperimental = GL_TRUE;
		const GLenum glewStatus = glewInit();

    GL_ERROR("glewInit()  [Safe to ignore]");

		if (glewStatus != GLEW_OK)
		{
			const std::string glewError = std::string((char*)glewGetErrorString(glewStatus));
			CaffUtil::LogInfo(glewError);
			// Throw?
		}
	}
	#endif

	// Setup Open GL
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	GL_ERROR("Renderer::Renderer()");
}


void Device::bind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, m_viewPortWidth, m_viewPortHeight);

}


void Device::clear(const bool color, const bool depth)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  GLenum flags = 0;
  if(color) { flags |= GL_COLOR_BUFFER_BIT; }
  if(depth) { flags |= GL_DEPTH_BUFFER_BIT; }

  glClear(flags);
}



void Device::setViewPort(const uint32_t width, const uint32_t height)
{
  m_viewPortWidth = static_cast<GLsizei>(width);
  m_viewPortHeight = static_cast<GLsizei>(height);
}


} // namespace Dev
} // namespace
} // namespace
