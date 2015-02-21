
#include <Caffeine/Application/Renderer/Shader.hpp>
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp> // kill me?
#include <Caffeine/Application/Renderer/VertexBuffer.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <Caffeine/Math/Math.hpp>


namespace
{
  std::string praseShader(const std::string &combinedShader, const std::string &tagName)
  {
    const std::string openTag  = "[" + tagName + "]";
    const std::string closeTag = "[/" + tagName + "]";

    const std::size_t shaderStart = combinedShader.find(openTag);
    const std::size_t shaderEnd = combinedShader.find(closeTag);

    if((shaderStart != std::string::npos) && (shaderEnd != std::string::npos))
    {
      const std::size_t start = shaderStart + openTag.size();
      const std::size_t size = shaderEnd - start;

      const std::string shader = combinedShader.substr(start, size);

      return shader;
    }

    return "";
  }

  enum class ConstantType
  {
	  FLOAT,
	  VEC2,
	  VEC3,
	  VEC4,
	  INT,
	  IVEC2,
	  IVEC3,
	  IVEC4,
	  BOOL,
	  BVEC2,
	  BVEC3,
	  BVEC4,
	  MAT2,
	  MAT3,
	  MAT4,
	
	  size,
  };

	ConstantType getGLConstantType(GLenum type)
	{
			switch (type){
				case GL_FLOAT:      return ConstantType::FLOAT;
				case GL_FLOAT_VEC2: return ConstantType::VEC2;
				case GL_FLOAT_VEC3: return ConstantType::VEC3;
				case GL_FLOAT_VEC4: return ConstantType::VEC4;
				case GL_INT:        return ConstantType::INT;
				case GL_INT_VEC2:   return ConstantType::IVEC2;
				case GL_INT_VEC3:   return ConstantType::IVEC3;
				case GL_INT_VEC4:   return ConstantType::IVEC4;
				case GL_BOOL:       return ConstantType::BOOL;
				case GL_BOOL_VEC2:  return ConstantType::BVEC2;
				case GL_BOOL_VEC3:  return ConstantType::BVEC3;
				case GL_BOOL_VEC4:  return ConstantType::BVEC4;
				case GL_FLOAT_MAT2: return ConstantType::MAT2;
				case GL_FLOAT_MAT3: return ConstantType::MAT3;
				case GL_FLOAT_MAT4: return ConstantType::MAT4;
			}

		return (ConstantType) -1;
	}


} // anon namespace


namespace Caffeine {
namespace Application {
namespace Dev {


Shader::Shader(const std::string &combinedShader)
: Shader(praseShader(combinedShader, "VERTEXSHADER"), praseShader(combinedShader, "PIXELSHADER"), praseShader(combinedShader, "GEOMETRYSHADER"))
{
}


Shader::Shader(const std::string &vertShader, const std::string &pixelShader, const std::string &geometryShader)
{
  loadShader(vertShader, pixelShader, geometryShader);
}


Shader::~Shader()
{
  if(m_vertexID)    { glDeleteShader(m_vertexID);   }
  if(m_geometryID)  { glDeleteShader(m_geometryID); }
  if(m_fragmentID)  { glDeleteShader(m_fragmentID); }
  if(m_programID)   { glDeleteProgram(m_programID); }
}


void Shader::loadShader(const std::string &combinedShader)
{
  loadShader(praseShader(combinedShader, "VERTEXSHADER"), praseShader(combinedShader, "PIXELSHADER"), praseShader(combinedShader, "GEOMETRYSHADER"));
}


void Shader::loadShader(const std::string &vertShader, const std::string &pixelShader, const std::string &geometryShader)
{ 
  auto compileShader = [](const std::string &shaderCode, const GLenum shaderType) -> GLuint
  {
    if(shaderCode.empty())
    {
      return 0;
    }

    GLuint shaderID = glCreateShader(shaderType);

		const char * vertSource = shaderCode.c_str();
		glShaderSource(shaderID, 1, &vertSource, NULL);
		glCompileShader(shaderID);
		
		// Log
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		
		std::vector<GLchar> log;
		log.resize(logLength);
		glGetShaderInfoLog(shaderID, logLength, 0, &log[0]);

		if(logLength > 1)
		{
      std::string logStr;
      logStr.reserve(log.size());
      logStr.append(&log[0]);

		  // Did it compile
		  GLint isCompiled;
		  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

      if(isCompiled == GL_FALSE)
      {
        CaffUtil::LogError("Error(s) compiling vert shader.");
        CaffUtil::LogError(logStr);
        return 0;
      }
      else
      {
        CaffUtil::LogWarning("Warning(s) compiling shader.");
        CaffUtil::LogWarning(logStr);
      }
		}

    return shaderID;
  };

  m_vertexID    = compileShader(vertShader, GL_VERTEX_SHADER);
  m_geometryID  = compileShader(geometryShader, GL_GEOMETRY_SHADER);
  m_fragmentID  = compileShader(pixelShader, GL_FRAGMENT_SHADER);

  // Link the shader
  if(m_vertexID && m_fragmentID)
  {
    m_programID = glCreateProgram();
    
    glAttachShader(m_programID, m_vertexID);
    if(m_geometryID) { glAttachShader(m_programID, m_geometryID); }
    glAttachShader(m_programID, m_fragmentID);
    glLinkProgram(m_programID);

    // Check the log
    GLint logLength;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLength);
		
		std::vector<GLchar> log;
		log.resize(logLength);
		glGetProgramInfoLog(m_programID, logLength, 0, &log[0]);

    // Did it link
    GLint isLinked;
		glGetProgramiv(m_programID, GL_LINK_STATUS, &isLinked);

		if(logLength > 1)
		{
      std::string logStr;
      logStr.reserve(log.size());
      logStr.append(&log[0]);

      if(isLinked == GL_FALSE)
      {
        CaffUtil::LogError("Error(s) linking program.");
        CaffUtil::LogError(logStr);
      }
      else
      {
        CaffUtil::LogWarning("Warning(s) linking program.");
        CaffUtil::LogWarning(logStr);
      }
		}
    
    // Give up there
    if(isLinked == GL_FALSE)
    {
      m_programID = 0;
      return;
    }
  } // If linked
  else
  {
    return;
  }

  // Get uniforms
  {
    glUseProgram(m_programID);

    // Get uniforms.
    GLint uniformCount, uniformLength;
    glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &uniformCount);
    glGetProgramiv(m_programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformLength);

    m_samplers.reserve(uniformCount);
    m_constants.reserve(uniformCount);

    for(GLint i = 0; i < uniformCount; ++i)
    {
      GLenum glType = 0;
      GLint length  = 0;
      GLint size    = 0;

      std::vector<GLchar> uniName;
      uniName.reserve(uniformLength);

      glGetActiveUniform(m_programID, i, uniformLength, &length, &size, &glType, uniName.data());
      const std::string uniformName(uniName.data());

      // Is sampler?
      if((glType >= GL_SAMPLER_1D) && (glType <= GL_SAMPLER_2D_RECT_SHADOW))
      {
        const GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
        GL_ERROR("Getting uniform location");

        glUniform1i(location, m_samplers.size());
        
        m_samplers.emplace_back(Sampler{uniformName, m_samplers.size()});
      }
      // Then uniform
      else
      {
        const std::string prefix = "gl_";

        if(uniformName.compare(0, prefix.length(), prefix) != 0)
        {
          const GLint index = glGetUniformLocation(m_programID, uniformName.c_str());
          const auto type = getGLConstantType(glType);

          m_constants.emplace_back(Constant{uniformName, index, (int)type, size});
        }
      }
    }

    m_samplers.shrink_to_fit();
    m_constants.shrink_to_fit();

    m_pendingUploadData.reserve(m_constants.size() * (4 * sizeof(float))); // Huristic to resever space for data. constants * vec4
    m_pendingTextureData.reserve(m_constants.size());
    m_pendingUpload.reserve(m_constants.size());

  } // getting uniforms and samplers

  glUseProgram(0);
}


void Shader::setShaderRaw(const std::string &name, const std::size_t bytesOfData, const void *data)
{
  const auto it = std::find_if(std::begin(m_constants), std::end(m_constants), [name](const Constant &constant){ return constant.name == name; });

  if(it != m_constants.end())
  {    
    // Check to see if data is already pending.    
    const uint32_t index = it->index;
    const auto dataIt = std::find_if(std::begin(m_pendingUpload), std::end(m_pendingUpload), [index](const PendingUpload &upload){ return std::get<UPLOAD_INDEX>(upload) == index; });
      
    // If already have data then overwrite the data.
    if(dataIt != m_pendingUpload.end())
    {
        memcpy(&m_pendingUploadData.at(std::get<UPLOAD_START_INDEX>(*dataIt)), data, bytesOfData);
    }
    // Else push back into pending data.
    else
    {
      m_pendingUpload.emplace_back(PendingUpload{it->index, m_pendingUploadData.size(), it->nElements, it->type, bytesOfData});

      // Check resize container.
      const std::size_t sizeNeeded  = m_pendingUploadData.size() + bytesOfData;
      const std::size_t insertPoint = m_pendingUploadData.size();

      m_pendingUploadData.resize(sizeNeeded);
      memcpy(&m_pendingUploadData.at(insertPoint), data, bytesOfData);
    }
  }
}


void Shader::setTexture(const std::string &name, const Texture &texture)
{
  const auto it = std::find_if(std::begin(m_samplers), std::end(m_samplers), [name](const Sampler &sampler){ return sampler.name == name; });

  if(it != m_samplers.end())
  {
    m_pendingTextureData.emplace_back(PendingTexture{texture.getTextureID(), it->index});
  }
}


void Shader::setTexture(const std::string &name, const FrameBuffer &texture)
{
  const auto it = std::find_if(std::begin(m_samplers), std::end(m_samplers), [name](const Sampler &sampler){ return sampler.name == name; });

  if(it != m_samplers.end())
  {
    m_pendingTextureData.emplace_back(PendingTexture{texture.getTextureID(), it->index});
  }
}


void Shader::bind() const
{
  glUseProgram(m_programID);
  GL_ERROR("Failed to use program")

  // Constants 
  {
    for(const auto &upload : m_pendingUpload)
    {
      const ConstantType type             = (ConstantType)std::get<UPLOAD_TYPE>(upload);
      const uint32_t     index            = std::get<UPLOAD_INDEX>(upload);
      const uint32_t     numberOfElements = std::get<UPLOAD_NUM_ELEMENTS>(upload);
      const void         *data            = &(m_pendingUploadData.at(std::get<UPLOAD_START_INDEX>(upload)));

      switch(type)
      {
        case(ConstantType::FLOAT): 	glUniform1fv(index, numberOfElements, (GLfloat*)data);  break;
        case(ConstantType::VEC2): 	glUniform2fv(index, numberOfElements, (GLfloat*)data);  break;
        case(ConstantType::VEC3): 	glUniform3fv(index, numberOfElements, (GLfloat*)data);  break;
        case(ConstantType::VEC4): 	glUniform4fv(index, numberOfElements, (GLfloat*)data);  break;
        case(ConstantType::INT): 	  glUniform1iv(index, numberOfElements, (GLint*)data);    break;
        case(ConstantType::IVEC2): 	glUniform2iv(index, numberOfElements, (GLint*)data);    break;
        case(ConstantType::IVEC3): 	glUniform3iv(index, numberOfElements, (GLint*)data);    break;
        case(ConstantType::IVEC4): 	glUniform4iv(index, numberOfElements, (GLint*)data);    break;
        case(ConstantType::BOOL): 	glUniform1iv(index, numberOfElements, (GLint*)data);    break;
        case(ConstantType::BVEC2): 	glUniform2iv(index, numberOfElements, (GLint*)data);    break;
        case(ConstantType::BVEC3): 	glUniform3iv(index, numberOfElements, (GLint*)data);    break;
        case(ConstantType::BVEC4): 	glUniform4iv(index, numberOfElements, (GLint*)data);    break;
        case(ConstantType::MAT2): 	glUniformMatrix2fv(index, numberOfElements, GL_FALSE, (GLfloat*)data); break;
        case(ConstantType::MAT3): 	glUniformMatrix3fv(index, numberOfElements, GL_FALSE, (GLfloat*)data); break;
        case(ConstantType::MAT4): 	glUniformMatrix4fv(index, numberOfElements, GL_FALSE, (GLfloat*)data); break;
      };
    }
    
    m_pendingUpload.clear();
    m_pendingUploadData.clear();

  } // Constants

  // Textures
  {
    const std::size_t numberOfTextures = CaffMath::Min(m_pendingTextureData.size(), static_cast<std::size_t>(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS));

    for(std::size_t i = 0; i < numberOfTextures; ++i)
    {
      glActiveTexture(GL_TEXTURE0 + m_pendingTextureData.at(i).index);
      glBindTexture(GL_TEXTURE_2D, m_pendingTextureData.at(i).textureID);

  	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		  GL_ERROR("Failed binding tex.");
    }

    m_pendingTextureData.clear();
  }
}


} // Dev namespace
} // namespace
} // namespace