
#include <Caffeine/Application/Renderer/VertexBuffer.hpp>
#include <Caffeine/Application/Renderer/VertexFormat.hpp>
#include <Caffeine/Application/Renderer/Shader.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>




namespace Caffeine {
namespace Application {
namespace Dev {


VertexBuffer::VertexBuffer(const std::vector<float> &vertexData, const bool isDynamic)
{
  loadVertexBuffer(vertexData, isDynamic);
}


void VertexBuffer::loadVertexBuffer(const std::vector<float> &vertexData, const bool isDynamic)
{
  glGenBuffers(1, &m_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if(isValid())
  {
    m_bufferSize = static_cast<GLuint>(vertexData.size());
    m_triangles = vertexData.size() / 3;
  }

  GL_ERROR("Adding vertex buffer");
}


void VertexBuffer::bind(const VertexFormat &vertexFormat, const Shader &shader) const
{
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
  GL_ERROR("Failed binding vertex buffer");

  // Vertex Format
  {
    for(std::size_t i = 0; i < vertexFormat.getNumberOfElements(); ++i)
    {
      const GLint NOT_USED = -1;

      Attribute attrib = vertexFormat.getAttr(i);

      GLint index = glGetAttribLocation(shader.getProgramID(), attrib.name.c_str());

      if(index != NOT_USED)
      {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, attrib.size, attrib.type, GL_FALSE, vertexFormat.getStride(), (void*)attrib.pointer);

        GL_ERROR("Failed applying vertex format '" + attrib.name + "'")
      }
    }
  }

  GL_ERROR("Failed applying vertex buffer.");
}


} // namespace Dev
} // namespace
} // namespace