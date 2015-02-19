
#include <Caffeine/Application/Renderer/VertexFormat.hpp>


namespace
{
  std::size_t sizeOfType(CaffApp::Dev::AttrType type)
  {
    switch(type)
    {
      case(CaffApp::Dev::AttrType::FLOAT): return 1; break;
      case(CaffApp::Dev::AttrType::FLOAT2): return 2; break;
      case(CaffApp::Dev::AttrType::FLOAT3): return 3; break;
      case(CaffApp::Dev::AttrType::FLOAT4): return 4; break;
    }
  }
}


namespace Caffeine {
namespace Application {
namespace Dev {


VertexFormat::VertexFormat(const std::vector<AttributeFormatDesc> &desc)
{
  loadFormat(desc);
}


void VertexFormat::loadFormat(const std::vector<AttributeFormatDesc> &desc)
{
  m_stride = 0;

  for(const auto &attr : desc)
  {
    Attribute attribute;
    attribute.name    = attr.name;
    attribute.size    = sizeOfType(attr.type);
    attribute.type    = GL_FLOAT;
    attribute.pointer = m_stride;

    m_stride += (sizeOfType(attr.type) * sizeof(GLfloat));

    m_format.push_back(attribute);
  }
}


void VertexFormat::bind() const
{

}


} // namespace
} // namespace
} // namespace