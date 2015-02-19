
#include <Caffeine/Application/Renderer/RendererDev.hpp>
#include <Caffeine/Application/Renderer/Device.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Application/Renderer/Shader.hpp>
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <Caffeine/Application/Renderer/VertexBuffer.hpp>
#include <Caffeine/Application/Renderer/VertexFormat.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>


namespace
{
   void setShader(const CaffApp::Dev::Shader &shader)
   {

   }
}


namespace Caffeine {
namespace Application {
namespace Dev {
namespace Renderer {


void Reset()
{
  const GLuint NO_SHADER = 0;

  glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
  glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glUseProgram(NO_SHADER);
	
  GL_ERROR("Reseting")
}


void ResetToDefaultBuffer()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GL_ERROR("Reseting to default buffer.")
}


void Draw(const Device &device, const Shader &shader, const VertexFormat &vertexFormat, const VertexBuffer &vertexBuffer)
{
  // Order here is important.
  device.bind();
  shader.bind();
  vertexBuffer.bind(vertexFormat, shader);

  glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.getNumberOfTriangles());

  GL_ERROR("Drawing to device.")
}


void Draw(const FrameBuffer &frameBuffer, const Shader &shader, const VertexFormat &vertexFormat, const VertexBuffer &vertexBuffer)
{
  // Order here is important.
  frameBuffer.bind();
  shader.bind();
  vertexBuffer.bind(vertexFormat, shader);

  glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.getNumberOfTriangles());

  GL_ERROR("Drawing to frame buffer.")
}


//void Renderer::Draw()
//{
//}


} // namespace
} // namespace Dev
} // namespace
} // namespace