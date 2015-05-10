#ifndef AUDIO_MANAGER_INCLUDED_
#define AUDIO_MANAGER_INCLUDED_


#include <Caffeine/Application/audio/audio_manager.hpp>
#include <string>
#include <stdint.h>


namespace caffeine {
namespace audio {


bool          initialize();

uint32_t      create_sample(const std::string &filename);
uint32_t      create_music(const std::string &filename);

bool          is_sample_valid(const uint32_t id);

void          play_sample(const uint32_t id);
void          stop_sample(const uint32_t id);

void          set_sample_position(const uint32_t sample_id, const float x, const float y, const float z);

void          set_sample_volume();
void          get_sample_volume();



} // namespace
} // namespace


#endif // include guard