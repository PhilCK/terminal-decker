#ifndef LIGHTING_MANAGER_INCLUDED_E7A3C134_EDE5_4603_9482_565450762D25
#define LIGHTING_MANAGER_INCLUDED_E7A3C134_EDE5_4603_9482_565450762D25


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/NonCopyable.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <vector>


namespace Caffeine {
namespace Systems {


class light
{
  CaffMath::Vector3   color;
  CaffMath::Vector3   position;
  float               ambient_intensity;
  float               diffuse_intensity;

  float               constant;
  float               linear;
  float               exp;
};


class lighting_manager
{

	lighting_manager(const lighting_manager &) = delete;
	lighting_manager & operator=(const lighting_manager &) = delete;

public:

  explicit                lighting_manager(const std::size_t number_or_lights_hint = 1000);

  uint32_t                add_light(const light &light);
  void                    update_light(const uint32_t id, const light &light);
  light                   get_light(const uint32_t id) const;
  inline void*            get_data() { return static_cast<void*>(m_lights.data()); }
  std::size_t             get_number_of_bytes_in_data() const;

private:

  std::vector<light>      m_lights;

}; // class


} // namespace
} // namespace


#endif // include guard