
#include <Caffeine/Systems/lighting_manager.hpp>


namespace Caffeine {
namespace Systems {


lighting_manager::lighting_manager(const std::size_t number_or_lights_hint)
{
  m_lights.reserve(number_or_lights_hint);
}


uint32_t lighting_manager::add_light(const light &light)
{
  m_lights.push_back(light);

  return m_lights.size();
}


void lighting_manager::update_light(const uint32_t id, const light &light)
{
  m_lights.at(id - 1) = light;
}


light lighting_manager::get_light(const uint32_t id) const
{
  assert(id <= m_lights.size() && id != 0);

  return m_lights.at(id - 1);
}





} // namespace
} // namespace