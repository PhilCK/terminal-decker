#ifndef RENDERER_INCLUDED_D51D1647_31F2_4E0E_B1A4_F9C1FF620CB2
#define RENDERER_INCLUDED_D51D1647_31F2_4E0E_B1A4_F9C1FF620CB2

#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>

namespace Caffeine {
namespace Application {
namespace Dev {
namespace Renderer {

void Reset();

void ResetToDefaultBuffer();

void Draw(const Device &device, const Shader &shader, const VertexFormat &vertexFormat, const VertexBuffer &vertexBuffer);

void Draw(const FrameBuffer &frameBuffer, const Shader &shader, const VertexFormat &vertexFormat, const VertexBuffer &vertexBuffer);

} // namespace
} // namespace Dev
} // namespace
} // namespace

#endif // include guard