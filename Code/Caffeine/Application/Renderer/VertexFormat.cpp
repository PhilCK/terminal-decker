
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
      case(CaffApp::Dev::AttrType::INT): return 1; break;
      case(CaffApp::Dev::AttrType::UINT): return 1; break;
    }

    assert(false); // shouldn't have got here.
    return 0; // For waring message.
  }

  GLenum glType(CaffApp::Dev::AttrType type)
  {
    switch(type)
    {
      case(CaffApp::Dev::AttrType::FLOAT): return GL_FLOAT; break;
      case(CaffApp::Dev::AttrType::FLOAT2): return GL_FLOAT; break;
      case(CaffApp::Dev::AttrType::FLOAT3): return GL_FLOAT; break;
      case(CaffApp::Dev::AttrType::FLOAT4): return GL_FLOAT; break;
      case(CaffApp::Dev::AttrType::INT): return GL_INT; break;
      case(CaffApp::Dev::AttrType::UINT): return GL_UNSIGNED_INT; break;
    }

    assert(false); // shouldn't have got here.
    return 0; // For waring message.
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
    attribute.type    = glType(attr.type);
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