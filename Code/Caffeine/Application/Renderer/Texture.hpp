#ifndef TEXTURE_INCLUDED_556DB782_053D_48D0_BE2E_37C79F830874
#define TEXTURE_INCLUDED_556DB782_053D_48D0_BE2E_37C79F830874


#include <Caffeine/Common/Platform.hpp>
#include <string>
#include <vector>


namespace Caffeine {
namespace Application {
namespace Dev {


class Texture
{
public:

  explicit          Texture() {}
  explicit          Texture(const std::string &filename);
  explicit          Texture(const std::vector<float> &data);
                    ~Texture();

  void              loadTexture(const std::string &filename);
  void              loadTexture(const std::vector<float> &data);
  void              updateSubset(const std::vector<float> &data, const uint32_t offsetX, const uint32_t offsetY);

  inline bool       isValid() { return m_textureID > 0; }

  inline GLuint     getTextureID() const { return m_textureID; }

private:


  uint32_t          m_width       = 0;
  uint32_t          m_height      = 0;
  uint32_t          m_flags       = 0;
  GLuint            m_textureID   = 0;
  GLuint            m_target      = 0;

}; // class


} // namespace Dev
} // namespace
} // namespace


#endif // include guard