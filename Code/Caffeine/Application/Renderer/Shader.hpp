#ifndef SHADER_INCLUDED_AD67BA55_6E70_41E4_A7B0_C694696F139B
#define SHADER_INCLUDED_AD67BA55_6E70_41E4_A7B0_C694696F139B


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Application/Renderer/RendererFwd.hpp>
#include <Caffeine/Application/Renderer/VertexFormat.hpp>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <streambuf> 


namespace Caffeine {
namespace Application {
namespace Dev {

namespace ShaderUtil
{
  inline std::string GetShaderCodeFromFile(const std::string &filename)
  {
    const std::string shaderCode(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());
    return shaderCode;
  }
}

class Shader
{
public:

  explicit            Shader() {}
  explicit            Shader(const std::string &combinedShader);
  explicit            Shader(const std::string &vertShader, const std::string &pixelShader, const std::string &geometryShader = "");
                      ~Shader();

  void                loadShader(const std::string &combinedShader);
  void                loadShader(const std::string &vertShader, const std::string &pixelShader, const std::string &geometryShader = "");

  inline void         setShader1f(const std::string &name, float data)                                { setShaderRaw(name, sizeof(float), &data);         }
  inline void         setShader2f(const std::string &name, const std::array<float,2> &data)           { setShaderRaw(name, sizeof(float) * 2, &data[0]);  }
  inline void         setShader3f(const std::string &name, const std::array<float,3> &data)           { setShaderRaw(name, sizeof(float) * 3, &data[0]);  }
  inline void         setShader4f(const std::string &name, const std::array<float,4> &data)           { setShaderRaw(name, sizeof(float) * 4, &data[0]);  }
  inline void         setShaderMatrix33f(const std::string &name, const std::array<float,9> &data)    { setShaderRaw(name, sizeof(float) * 9, &data[0]);  }
  inline void         setShaderMatrix44f(const std::string &name, const std::array<float,16> &data)   { setShaderRaw(name, sizeof(float) * 16, &data[0]); }
  void                setShaderRaw(const std::string &name, const std::size_t bytesOfData, const void* data);
  void                setTexture(const std::string &name, const Texture &texture);
  void                setTexture(const std::string &name, const FrameBuffer &texture);

  inline bool         isValid() const { return m_programID > 0; }
  void                bind() const;

  inline GLuint       getProgramID() const    { return m_programID;   }
  inline GLuint       getVertexID() const     { return m_vertexID;    }
  inline GLuint       getGeometryID() const   { return m_geometryID;  }
  inline GLuint       getFragmentID() const   { return m_fragmentID;  }

private:

  enum { UPLOAD_INDEX, UPLOAD_START_INDEX, UPLOAD_NUM_ELEMENTS, UPLOAD_TYPE, UPLOAD_NUM_BYTES };
  using PendingUpload = std::tuple<uint32_t, uint32_t, uint32_t, int32_t, uint32_t>;
  
  struct Constant
  {
  	std::string name;
	  uint32_t    index;
	  uint32_t    type;
	  uint32_t    nElements;
  };

  struct Sampler
  {
    std::string name;
    uint32_t    index;
  };

  struct PendingTexture
  {
    GLuint    textureID;
    GLenum    target;
    uint32_t  index;
  };

  std::vector<Constant> m_constants;
	std::vector<Sampler>  m_samplers;

  mutable std::vector<PendingUpload>  m_pendingUpload;
  mutable std::vector<uint8_t>        m_pendingUploadData;
  mutable std::vector<PendingTexture> m_pendingTextureData;

  GLuint m_programID        = 0;
  GLuint m_vertexID         = 0;
  GLuint m_geometryID       = 0;
  GLuint m_fragmentID       = 0;

}; // class

} // Dev namespace
} // namespace
} // namespace


#endif // include guard