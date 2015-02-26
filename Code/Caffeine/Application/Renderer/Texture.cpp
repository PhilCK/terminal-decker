
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


Texture::Texture(const std::vector<float> &data)
{
  loadTexture(data);
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

  m_width  = width;
  m_height = height;

  glGenTextures(1, &m_textureID);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);

  glBindTexture(GL_TEXTURE_2D, 0);

  GL_ERROR("Adding texture.");
}


void Texture::loadTexture(const std::vector<float> &data)
{
  const uint32_t size = CaffMath::SquareRoot(data.size() / 4);

  m_width  = size;
  m_height = size;

  glGenTextures(1, &m_textureID);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

  glBindTexture(GL_TEXTURE_2D, 0);

  GL_ERROR("Adding texture.");
}


Texture::~Texture()
{
  glDeleteTextures(1, &m_textureID);
}


} // Dev namespace
} // namespace
} // namespace