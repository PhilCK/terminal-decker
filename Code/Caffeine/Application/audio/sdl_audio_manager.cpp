#include <Caffeine/Application/audio/audio_manager.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <mutex>


namespace
{
  struct audio_sample
  {
    Mix_Chunk *chunk  = nullptr;
    uint32_t channel  = 0;
    float x           = 0.f;
    float y           = 0.f;
    float z           = 0.f;
  };

  struct audio_track
  {
    Mix_Chunk *chunk = nullptr;
    uint32_t channel = 0;
  };
  
  std::vector<audio_sample> audio_sample_tracks;
  std::vector<audio_track> audio_music_tracks;
  
  uint32_t size_to_id(uint32_t i) { return i;     }
  uint32_t id_to_size(uint32_t i) { return i - 1; }
  
  std::mutex lock;
}


namespace caffeine {
namespace audio {


bool
initialize()
{
  std::lock_guard<std::mutex> locker(lock);

  auto error = [](const std::string &error_msg)
  {
    const std::string error_code = Mix_GetError();
    CaffUtil::LogError(error_msg + " " + error_code);
    
    return false;
  };

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
  {
    error("SDL_mixer could not init");
  }
  
  if(Mix_AllocateChannels(2048) < 0)
  {
    error("SDL_mixer could not allocate channels");
  }
  
  // Allocate memory last.
  {
    audio_sample_tracks.reserve(1024);
    audio_music_tracks.reserve(1024);
  }
  
  return true;
}


uint32_t
create_sample(const std::string &filename)
{
  std::lock_guard<std::mutex> locker(lock);

  audio_sample sample;
  sample.chunk = Mix_LoadWAV(filename.c_str());
  
  if(sample.chunk)
  {
    const uint32_t size = static_cast<uint32_t>(audio_sample_tracks.size());
  
    sample.channel = size + 1;
  
    audio_sample_tracks.emplace_back(sample);
    return size_to_id(size + 1);
  }
  else
  {
    return 0;
  }
}


bool
is_sample_valid(const uint32_t id)
{
  return id > 0;
}


void
play_sample(const uint32_t sample_id)
{
  std::lock_guard<std::mutex> locker(lock);

  const auto &sample = audio_sample_tracks.at(id_to_size(sample_id));
  
  Mix_PlayChannel(sample_id, sample.chunk, 0);
}


void
set_sample_position(const uint32_t sample_id, const float x, const float y, const float z)
{
  std::lock_guard<std::mutex> locker(lock);
  
  auto &sample = audio_sample_tracks.at(id_to_size(sample_id));
  
  sample.x = x;
  sample.y = y;
  sample.z = z;
}


} // namespace
} // namespace