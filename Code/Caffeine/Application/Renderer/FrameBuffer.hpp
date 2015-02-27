#ifndef FRAME_BUFFER_INCLUDED_E644CA1B_5ED7_4F57_B2A9_F53974281DFF
#define FRAME_BUFFER_INCLUDED_E644CA1B_5ED7_4F57_B2A9_F53974281DFF


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>
#include <vector>
#include <string>


namespace Caffeine {
namespace Application {
namespace Dev {


class FrameBuffer
{
public:

  explicit              FrameBuffer() {}
  explicit              FrameBuffer(const uint32_t width, const uint32_t height);
                        ~FrameBuffer();

  void                  loadBuffer(const uint32_t width, const uint32_t height);
  inline bool           isValid() const { return m_frameBuffer > 0; }

  inline uint32_t       getWidth() const  { return m_width; }
  inline uint32_t       getHeight() const { return m_height; }

  void                  bind() const;
  void                  unbind() const;
  void                  clear(const bool color = true, const bool depth = true);
  void                  setViewPort(const uint32_t width, const uint32_t height);

  GLuint                getTextureID() const { return m_colorBuffer; }

private:

  uint32_t              m_width             = 0;
  uint32_t              m_height            = 0;
  GLsizei               m_viewPortWidth     = 0;
  GLsizei               m_viewPortHeight    = 0;
  GLuint                m_frameBuffer       = 0;
  GLuint                m_colorBuffer       = 0;
  GLuint                m_renderBuffer      = 0;

}; // class


} // namespace Dev
} // namespace
} // namespace


#endif // include guard