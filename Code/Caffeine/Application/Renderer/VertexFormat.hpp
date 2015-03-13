#ifndef VERTEX_FORMAT_INCLUDED_5B07E375_431F_439C_ACB9_FC19549D9F0A
#define VERTEX_FORMAT_INCLUDED_5B07E375_431F_439C_ACB9_FC19549D9F0A


#include <Caffeine/Common/Platform.hpp>
#include <vector>
#include <string>


namespace Caffeine {
namespace Application {
namespace Dev {

// TODO support for int/uint types needs finish test.
enum class AttrType { FLOAT, FLOAT2, FLOAT3, FLOAT4, INT, UINT, };


struct AttributeFormatDesc
{
  std::string name;
  AttrType    type;
};


struct Attribute
{
  std::string name;
  std::size_t size;
  std::size_t pointer;
  GLenum      type;
};


class VertexFormat
{
public:

  explicit                VertexFormat() {}
  explicit                VertexFormat(const std::vector<AttributeFormatDesc> &desc);
                          ~VertexFormat() {}

  void                    loadFormat(const std::vector<AttributeFormatDesc> &desc);
  inline bool             hasFormatedLoaded() const { return !m_format.empty(); }

  inline std::size_t      getNumberOfElements() const { return m_format.size(); }
  inline uint32_t         getStride() const { return m_stride; }

  Attribute               getAttr(const int i) const { return m_format.at(i); }

  void                    bind() const;

private:

  std::vector<Attribute>  m_format;
  uint32_t                m_stride      = 0;

}; // class


} // namespace Dev
} // namespace
} // namespace


#endif // include guard