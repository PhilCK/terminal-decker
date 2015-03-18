
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <Caffeine/Math/Math.hpp>
#include <SOIL/SOIL.h>


namespace
{
  #ifndef NDEBUG
 
  bool debugInfo = false;

  GLint debugMaxTexSize = 0;
  GLint debugMaxTexBufferSize = 0;
  

  void GetDebugInfo()
  {
    if(debugInfo) { return; }

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &debugMaxTexSize);
    glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &debugMaxTexBufferSize);

    GL_ERROR("Error while getting debug info.")

    debugInfo = true;
  }

  #endif
}


namespace Caffeine {
namespace Application {
namespace Dev {


Texture::Texture(const std::string &filename)
{
  loadTexture(filename);

  #ifndef NDEBUG
  GetDebugInfo();
  #endif
}


Texture::Texture(const std::vector<float> &data, const TextureD dimention, const Format format, const uint32_t width, const uint32_t height)
{
  loadTexture(data, dimention, format, width, height);

  #ifndef NDEBUG
  GetDebugInfo();
  #endif
}


Texture::Texture(const std::vector<uint8_t> &data, const TextureD dimention, const Format format, const uint32_t width, const uint32_t height)
{
  loadTexture(data, dimention, format, width, height);

  #ifndef NDEBUG
  GetDebugInfo();
  #endif
}


namespace
{
  GLenum GetDimentionTarget(const TextureD dimention)
  {
    switch(dimention)
    {
    case(TextureD::ONE_D):
      return GL_TEXTURE_2D;
      break;
    case(TextureD::TWO_D):
      return GL_TEXTURE_2D;
      break;
    default:
      assert(false);
    }
  }


  void GenerateTexture(GLuint &textureID, const CaffApp::Dev::Format format, const CaffApp::Dev::TextureD dimention, const uint32_t width, const uint32_t height, void *data)
  {
    using namespace CaffApp::Dev;

    const auto externalFormat = format == Format::DEV ? GL_RGBA32F : GL_RGBA;
    const auto internalFormat = GL_RGBA;
    const auto formatType     = format == Format::R8G8B8 ? GL_UNSIGNED_BYTE : GL_FLOAT;
    const auto glDimention    = GetDimentionTarget(dimention);

    const GLsizei texWidth  = CaffMath::Max(width, static_cast<uint32_t>(1));
    const GLsizei texHeight = CaffMath::Max(height, static_cast<uint32_t>(1));

    assert(CaffMath::IsPOW2(texWidth));
    assert(CaffMath::IsPOW2(texHeight));

    glGenTextures(1, &textureID);
    glBindTexture(glDimention, textureID);

    switch(glDimention)
    {
      case(GL_TEXTURE_1D):
      {
        glTexImage1D(GL_TEXTURE_1D, 0, externalFormat, width, 0, internalFormat, formatType, data);
        break;
      }
      case(GL_TEXTURE_2D):
      {
        glTexImage2D(GL_TEXTURE_2D, 0, externalFormat, width, height, 0, internalFormat, formatType, data);
        break;
      }
      default:
        assert(false); // What did you do!
    };

    glBindTexture(glDimention, 0);
    GL_ERROR("Failed generating texture.") 
  }


  void UpdateTexture(const GLuint textureID, const CaffApp::Dev::Format format, const CaffApp::Dev::TextureD dimention, const uint32_t offsetX, const uint32_t offsetY, const uint32_t width, const uint32_t height, void *data)
  {
    using namespace CaffApp::Dev;

    const auto externalFormat = format == Format::DEV ? GL_RGBA32F : GL_RGBA ;
    const auto internalFormat = GL_RGBA;
    const auto formatType     = format == Format::R8G8B8 ? GL_UNSIGNED_BYTE : GL_FLOAT;
    const auto glDimention    = GetDimentionTarget(dimention);

    const GLsizei texWidth  = CaffMath::Max(width, static_cast<uint32_t>(1));
    const GLsizei texHeight = CaffMath::Max(height, static_cast<uint32_t>(1));
   
    assert(CaffMath::IsPOW2(texWidth));
    assert(CaffMath::IsPOW2(texHeight));

    glBindTexture(glDimention, textureID);

    switch(glDimention)
    {
      case(GL_TEXTURE_1D):
      {
        glTexSubImage1D(GL_TEXTURE_1D, offsetX, 0, texWidth, internalFormat, formatType, data);
        break;
      }
      case(GL_TEXTURE_2D):
      {
        glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, texWidth, texHeight, internalFormat, formatType, data);
        break;
      }
      default:
        assert(false); // What did you do!
    };

    glBindTexture(glDimention, 0);
    GL_ERROR("Updating texture.")
  }
}



void Texture::loadTexture(const std::string &filename)
{
  int width, height;
  unsigned char *image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

  if(!image)
  {
    CaffUtil::LogError("Failed to load image '" + filename + "'");
    SOIL_free_image_data(image);
    return;
  }

  m_width     = width;
  m_height    = height;
  m_format    = Format::R8G8B8;
  m_dimention = TextureD::TWO_D;

  GenerateTexture(m_textureID, m_format, m_dimention, m_width, m_height, image);

  SOIL_free_image_data(image);

  GL_ERROR("Adding texture.")
}


namespace
{
  void SetTextureDimentionFromDataSize(const TextureD dimention, const std::size_t dataSize, uint32_t &outWidth, uint32_t &outHeight)
  {
    assert(CaffMath::IsPOW2(dataSize)); // only pow2 square textures.

    switch(dimention)
    {
    case(TextureD::ONE_D):
       outWidth = dataSize;
       outHeight = 1;
       break;
    case(TextureD::TWO_D):
      outWidth = CaffMath::SquareRoot(dataSize / 4);
      outHeight = outWidth;
      break;
    default:
      assert(false); // Shouldn't have happened.
    }
  }
}


void Texture::loadTexture(const std::vector<float> &data, const TextureD dimention, const Format format, const uint32_t width, const uint32_t height)
{
  if(m_textureID)
  {
    CaffUtil::LogError("Texture already loaded.");
    return;
  }

  m_format    = format;
  m_dimention = dimention;

  if(width == 0 || height == 0)
  {
    SetTextureDimentionFromDataSize(m_dimention, data.size(), m_width, m_height);
  }
  else
  {
    assert(CaffMath::IsPOW2(width));
    assert(CaffMath::IsPOW2(height));

    m_width = width;
    m_height = height;
  }

  GenerateTexture(m_textureID, m_format, m_dimention, m_width, m_height, (void*)data.data());

  GL_ERROR("Failed loading texture");
}



void Texture::loadTexture(const std::vector<uint8_t> &data, const TextureD dimention, const Format format, const uint32_t width, const uint32_t height)
{
  const std::vector<float> floatData(data.begin(), data.end());
  loadTexture(floatData, dimention, format, width, height);
}


void Texture::updateSubset(const std::vector<float> &data, const uint32_t offsetX, const uint32_t offsetY)
{
  assert(m_textureID);
  assert(CaffMath::IsPOW2(data.size()));

  uint32_t sizeX = m_width;
  uint32_t sizeY = m_height;

  if(sizeX == 0 || sizeY == 0)
  {
    SetTextureDimentionFromDataSize(m_dimention, data.size(), sizeX, sizeY);
  }

  UpdateTexture(m_textureID, m_format, m_dimention, offsetX, offsetY, sizeX, sizeY, (void*)data.data());

  GL_ERROR("Updating texture.")
}

void Texture::updateSubset(const std::vector<uint8_t> &data, const uint32_t offsetX, const uint32_t offsetY)
{
  const std::vector<float> floatData(data.begin(), data.end());
  updateSubset(floatData, offsetX, offsetY);
}


GLenum Texture::getGLDimention() const
{
  return GetDimentionTarget(m_dimention);
}


Texture::~Texture()
{
  glDeleteTextures(1, &m_textureID);
}


} // Dev namespace
} // namespace
} // namespace