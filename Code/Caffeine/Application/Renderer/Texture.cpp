
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <Caffeine/Math/Math.hpp>
#include <SOIL/SOIL.h>

namespace Caffeine {
namespace Application {
namespace Dev {


Texture::Texture(const std::string &filename)
{
  loadTexture(filename);
}


Texture::Texture(const std::vector<float> &data, const TextureD dimention, const Format format)
{
  loadTexture(data, dimention, format);
}


Texture::Texture(const std::vector<uint8_t> &data, const TextureD dimention, const Format format)
{
  loadTexture(data, dimention, format);
}


namespace
{
  void GenerateTexture(GLuint &textureID, const CaffApp::Dev::Format format, const CaffApp::Dev::TextureD dimention, const uint32_t width, const uint32_t height, void *data)
  {
    using namespace CaffApp::Dev;

    const auto externalFormat = format == Format::DEV ? GL_RGBA32F : GL_RGBA ;
    const auto internalFormat = GL_RGBA;
    const auto formatType     = format == Format::R8G8B8 ? GL_UNSIGNED_BYTE : GL_FLOAT;
    const auto glDimention    = dimention == TextureD::ONE_D ? GL_TEXTURE_1D : GL_TEXTURE_2D;

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
    GL_ERROR("Failed generating 2D texture.") 
  }


  void UpdateTexture(const GLuint textureID, const CaffApp::Dev::Format format, const CaffApp::Dev::TextureD dimention, const uint32_t offsetX, const uint32_t offsetY, const uint32_t width, const uint32_t height, void *data)
  {
    using namespace CaffApp::Dev;

    const auto externalFormat = format == Format::DEV ? GL_RGBA32F : GL_RGBA ;
    const auto internalFormat = GL_RGBA;
    const auto formatType     = format == Format::R8G8B8 ? GL_UNSIGNED_BYTE : GL_FLOAT;
    const auto glDimention    = dimention == TextureD::ONE_D ? GL_TEXTURE_1D : GL_TEXTURE_2D;

    glBindTexture(glDimention, textureID);

    switch(glDimention)
    {
      case(GL_TEXTURE_1D):
      {
        glTexSubImage1D(GL_TEXTURE_1D, offsetX, 0, width, internalFormat, formatType, data);
        break;
      }
      case(GL_TEXTURE_2D):
      {
        glTexSubImage2D(GL_TEXTURE_2D, offsetX, offsetY, 0, width, height, internalFormat, formatType, data);
        break;
      }
      default:
        assert(false); // What did you do!
    };

    glBindTexture(glDimention, 0);
    GL_ERROR("Updating texture.")
  }

  GLenum GetDimentionTarget(const TextureD dimention)
  {
    switch(dimention)
    {
    case(TextureD::ONE_D):
      return GL_TEXTURE_1D;
      break;
    case(TextureD::TWO_D):
      return GL_TEXTURE_2D;
      break;
    default:
      assert(false);
    }
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

  GL_ERROR("Adding texture.");
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


void Texture::loadTexture(const std::vector<float> &data, const TextureD dimention, const Format format)
{
  if(m_textureID)
  {
    CaffUtil::LogError("Texture already loaded.");
    return;
  }

  m_format    = format;
  m_dimention = dimention;

  SetTextureDimentionFromDataSize(m_dimention, data.size(), m_width, m_height);
  GenerateTexture(m_textureID, m_format, m_dimention, m_width, m_height, (void*)data.data());

  GL_ERROR("Failed loading texture");
}


void Texture::loadTexture(const std::vector<uint8_t> &data, const TextureD dimention, const Format format)
{
  const std::vector<float> floatData(data.begin(), data.end());
  loadTexture(floatData, dimention, format);
}


void Texture::updateSubset(const std::vector<float> &data, const uint32_t offsetX, const uint32_t offsetY)
{
  assert(m_textureID);
  assert(CaffMath::IsPOW2(data.size()));

  uint32_t sizeX = 0;
  uint32_t sizeY = 0;

  SetTextureDimentionFromDataSize(m_dimention, data.size(), sizeX, sizeY);
  UpdateTexture(m_textureID, m_format, m_dimention, offsetX, offsetY, sizeX, sizeY, (void*)data.data());

  GL_ERROR("Updating texture.")
}

void Texture::updateSubset(const std::vector<uint8_t> &data, const uint32_t offsetX, const uint32_t offsetY)
{
  const std::vector<float> floatData(data.begin(), data.end());
  updateSubset(floatData, offsetX, offsetY);
}


Texture::~Texture()
{
  glDeleteTextures(1, &m_textureID);
}


} // Dev namespace
} // namespace
} // namespace