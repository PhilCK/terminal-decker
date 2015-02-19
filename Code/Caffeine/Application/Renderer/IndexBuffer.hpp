#ifndef INDEX_BUFFER_INCLUDED_16F1DF5F_D524_4A97_80DA_B06E6F0B68E9
#define INDEX_BUFFER_INCLUDED_16F1DF5F_D524_4A97_80DA_B06E6F0B68E9


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>
#include <vector>


namespace Caffeine {
namespace Application {
namespace Dev {


class IndexBuffer
{
public:
 
  explicit                IndexBuffer() {}
  explicit                IndexBuffer(const std::vector<uint32_t> &indexBuffer, const bool isDynamic = false);
                          ~IndexBuffer() {}

  void                    loadIndexBuffer(const std::vector<uint32_t> &indexBuffer, const bool isDynamic = false);

  inline bool             isValid() const { return m_indexBuffer > 0; }
  void                    bind() const;
  
  inline std::size_t      getNumberOfIndices() const { return m_numberOfIndices; }

private:

  GLuint                  m_indexBuffer       = 0;
  std::size_t             m_numberOfIndices   = 0;

};


} // namespace Dev
} // namespace
} // namespace


#endif // include guard