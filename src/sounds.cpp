#include "SDL3/SDL.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include "sounds.h"
#include "print.h"
#include "path.h"

sound::sound(std::string _path) {
    if (!_path.ends_with(".wav")) {
        print::error("Wrong format of sound!", _path);
    }
    m_path = _path;
    if (!SDL_LoadWAV(path(_path).c_str(), &m_filespec, &m_data, &m_size)) {
        print::error("Failed to open sound!", SDL_GetError());
    }
}

sound::~sound() {
    SDL_free(m_data);
}

SDL_AudioSpec audio_engine::m_specs;
SDL_AudioStream* audio_engine::m_stream = nullptr;
std::map<std::string, std::unique_ptr<sound>> audio_engine::m_sounds;
std::vector<SDL_AudioStream*> audio_engine::m_active_streams;

int audio_engine::frames_before_stream_check = 0;

audio_engine::audio_engine()
{
    m_active_streams.reserve(128);
    m_specs.format = SDL_AUDIO_F32;
    m_specs.channels = 2;
    m_specs.freq = 44100;

    m_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_specs, NULL, NULL);
    if (!m_stream) {
        print::error("Failed to open audio stream!", SDL_GetError());
    }
    SDL_ResumeAudioStreamDevice(m_stream);
}

audio_engine::~audio_engine()
{
    unload_sounds();
}

void audio_engine::load_sounds()
{
	print::loading("Loading sounds");
	print::increase_level();

	using json = nlohmann::json;
	std::ifstream file(path("sounds/sounds.json"));
	auto json_textures = json::parse(file);
	file.close();

	int count = json_textures.size();
	int i = 0;
	
	for (auto& [key, value] : json_textures.items()) {
		print::loading(std::format("Loading sound {}/{}: {}", ++i, count, key));
		m_sounds[key] = std::make_unique<sound>("sounds/" + std::string(value));
	}

	print::decrease_level();
	print::loaded("Sounds loaded");
}

void audio_engine::unload_sounds() {
    for (auto& stream : m_active_streams) {
        if (stream) {
            SDL_DestroyAudioStream(stream);
        }
    }
    m_active_streams.clear();

    m_sounds.clear();
    if (m_stream) {
        SDL_DestroyAudioStream(m_stream);
        m_stream = nullptr;
    }
}

sound* audio_engine::get_sound(std::string name)
{
    return m_sounds.at(name).get();
}

void audio_engine::play_sound(std::string name)
{
    auto sound = get_sound(name);
    if (sound->is_init()) {
        SDL_AudioStream* playback_stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, 
        sound->get_spec(),
        NULL, NULL);
        if (!playback_stream) {
            print::warning("Failed to play audio, failed to init temp stream!", name);
            return;
        }
        m_active_streams.push_back(playback_stream);
        
        if (!SDL_PutAudioStreamData(playback_stream, sound->get_data(), sound->get_size())) {
            print::warning("Cant play audio!", name);
            return;
        }
        SDL_FlushAudioStream(playback_stream);
        SDL_ResumeAudioStreamDevice(playback_stream);
    }
}

void audio_engine::check_active_streams()
{
    frames_before_stream_check++;
    if (frames_before_stream_check > 60) {
        std::erase_if(m_active_streams, [](SDL_AudioStream* s){
            if (SDL_GetAudioStreamQueued(s) <= 0) {
                SDL_DestroyAudioStream(s);
                return true;
            }
            return false;
        });
        frames_before_stream_check = 0;
    }

}