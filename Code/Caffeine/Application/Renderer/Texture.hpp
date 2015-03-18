#ifndef TEXTURE_INCLUDED_556DB782_053D_48D0_BE2E_37C79F830874
#define TEXTURE_INCLUDED_556DB782_053D_48D0_BE2E_37C79F830874


#include <Caffeine/Common/Platform.hpp>
#include <string>
#include <vector>


namespace Caffeine {
namespace Application {
namespace Dev {

enum class TextureD
{
  ONE_D,
  TWO_D,
};

enum class Format
{
  R8G8B8,
  R32G32B32,
  DEV,
};

class Texture
{
public:

  explicit            Texture() {}
  explicit            Texture(const std::string &filename);
  explicit            Texture(const std::vector<float> &data, const TextureD dimention = TextureD::TWO_D, const Format format = Format::R32G32B32, const uint32_t width = 0, const uint32_t height = 0);
  explicit            Texture(const std::vector<uint8_t> &data, const TextureD dimention = TextureD::TWO_D, const Format format = Format::R8G8B8, const uint32_t width = 0, const uint32_t height = 0);
                      ~Texture();

  void                loadTexture(const std::string &filename);
  void                loadTexture(const std::vector<float> &data, const TextureD dimention = TextureD::TWO_D, const Format format = Format::R32G32B32, const uint32_t width = 0, const uint32_t height = 0);
  void                loadTexture(const std::vector<uint8_t> &data, const TextureD dimention = TextureD::TWO_D, const Format format = Format::R8G8B8, const uint32_t width = 0, const uint32_t height = 0);

  void                updateSubset(const std::vector<float> &data, const uint32_t offsetX, const uint32_t offsetY);
  void                updateSubset(const std::vector<uint8_t> &data, const uint32_t offsetX, const uint32_t offsetY);

  inline bool         isValid() { return m_textureID > 0; }
  
  inline uint32_t     getWidth()  { return m_width;  }
  inline uint32_t     getHeight() { return m_height; }

  inline TextureD     getDimention() const    { return m_dimention; } // TODO: Should this return GLenum, DX isn't a thing for us.
  inline Format       getFormat() const       { return m_format;    } // TODO: Should this return GLenum, DX isn't a thing for us.

  // GL Helpers for prototyping.
  inline GLuint       getGLTextureID() const    { return m_textureID; }
  GLenum              getGLDimention() const; // We might use a different one internally.

private:

  uint32_t            m_width       = 0;
  uint32_t            m_height      = 0;
  uint32_t            m_flags       = 0;
  GLuint              m_textureID   = 0;
  Format              m_format      = Format::R32G32B32;
  TextureD            m_dimention   = TextureD::TWO_D;

}; // class


} // namespace Dev
} // namespace
} // namespace


#endif // include guard