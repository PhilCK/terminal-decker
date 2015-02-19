#ifndef VERTEX_BUFFER_INCLUDED_4479A0E1_489E_4D6E_A51A_C5E03739A092
#define VERTEX_BUFFER_INCLUDED_4479A0E1_489E_4D6E_A51A_C5E03739A092


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>
#include <vector>


namespace Caffeine {
namespace Application {
namespace Dev {


class VertexBuffer
{
public:

  explicit                VertexBuffer() {}
  explicit                VertexBuffer(const std::vector<float> &vertexData, const bool isDynamic = false);
                          ~VertexBuffer() {}

  void                    loadVertexBuffer(const std::vector<float> &vertexData, const bool isDynamic = false);

  inline bool             isValid() const { return m_vertexBuffer > 0; }
  void                    bind(const VertexFormat &vertexFmt, const Shader &shader) const;
  
  inline std::size_t      getNumberOfTriangles() const { return m_triangles; }

private:

  GLuint                  m_vertexBuffer      = 0;
  GLuint                  m_bufferSize        = 0;
  std::size_t             m_triangles         = 0;

};


} // namespace Dev
} // namespace
} // namespace


#endif // include guard