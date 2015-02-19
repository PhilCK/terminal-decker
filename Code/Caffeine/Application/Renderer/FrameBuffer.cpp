
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>




namespace Caffeine {
namespace Application {
namespace Dev {


FrameBuffer::FrameBuffer(const uint32_t width, const uint32_t height)
{
  loadBuffer(height, width);
}


FrameBuffer::~FrameBuffer()
{
  if(m_frameBuffer)   { glDeleteFramebuffers(1, &m_frameBuffer);    }
  if(m_renderBuffer)  { glDeleteRenderbuffers(1, &m_renderBuffer);  }
  if(m_colorBuffer)   { glDeleteTextures(1, &m_colorBuffer);        }
}

void FrameBuffer::loadBuffer(const uint32_t width, const uint32_t height)
{
  setViewPort(width, height);

  glGenFramebuffers(1, &m_frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

  glGenTextures(1, &m_colorBuffer);
  glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

  glGenRenderbuffers(1, &m_renderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

  GL_ERROR("FBO Stuff.")

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::bind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
  glViewport(0, 0, m_viewPortWidth, m_viewPortHeight);

  GL_ERROR("Binding FBO.")
}

void FrameBuffer::unbind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::clear(const bool color, const bool depth)
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

  GLenum flags = 0;
  if(color) { flags |= GL_COLOR_BUFFER_BIT; }
  if(depth) { flags |= GL_DEPTH_BUFFER_BIT; }

  glClear(flags);
}


void FrameBuffer::setViewPort(const uint32_t width, const uint32_t height)
{
  m_viewPortWidth = static_cast<GLsizei>(width);
  m_viewPortHeight = static_cast<GLsizei>(height);
}


} // namespace
} // namespace
} // namespace