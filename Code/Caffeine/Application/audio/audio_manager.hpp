#ifndef AUDIO_MANAGER_INCLUDED_728DAABC_0EC0_4600_BFF4_F3F4D0C0ECB7
#define AUDIO_MANAGER_INCLUDED_728DAABC_0EC0_4600_BFF4_F3F4D0C0ECB7


#include <Caffeine/Application/audio/audio_manager.hpp>
#include <string>
#include <stdint.h>


namespace caffeine {
namespace audio {


using sample_id = uint32_t;
using music_id = uint32_t;


bool          initialize();
void          think(const float dt);
void          deinitialize();


sample_id     create_sample(const std::string &filename);
music_id      create_music(const std::string &filename);


bool          is_sample_valid(const sample_id id);
void          play_sample(const sample_id id);
void          stop_sample(const sample_id id);


void          set_ear_position(const float x, const float y, const float z);
void          set_sample_position(const sample_id id, const float x, const float y, const float z);


void          set_sample_volume();
void          get_sample_volume();


} // namespace
} // namespace


#endif // include guard