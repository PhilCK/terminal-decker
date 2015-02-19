#ifndef DEVICE_INCLUDED_5541647A_4162_419D_BFB9_A53F67A5D3C7
#define DEVICE_INCLUDED_5541647A_4162_419D_BFB9_A53F67A5D3C7

#include <Caffeine/Common/Platform.hpp>

namespace Caffeine {
namespace Application {
namespace Dev {

class Device
{
public:

  explicit            Device(SDL_Window * sdlWindow);
  explicit            Device(const uint32_t width, const uint32_t height, const bool setFullscreen) {}
                      ~Device() {}

  void                setDeviceResolution(const uint32_t width, const uint32_t height, const bool setFullscreen);
  void                getWidth() const;
  void                getHeight() const;

  void                bind() const;
  void                setViewPort(const uint32_t width, const uint32_t height);

private:

  GLsizei             m_viewPortWidth = 0;
  GLsizei             m_viewPortHeight = 0;

  // SDL
  SDL_GLContext                     m_glContext;

  GLuint                            m_fboID = 0;
  GLuint                            m_vaoID = 0;

};

} // namespace Dev
} // namespace
} // namespace


#endif // include guard