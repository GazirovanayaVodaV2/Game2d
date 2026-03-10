#include "SDL3/SDL_audio.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include "sounds.h"
#include "print.h"
#include "path.h"

sound::sound(std::string _path) {
    if (!_path.ends_with(".wav")) {
        print::warning("Wrong format of sound!", _path);
        return;
    }
    m_path = _path;
    if (!SDL_LoadWAV(path(_path).c_str(), &m_filespec, &m_data, &m_size)) {
        print::warning("Failed to open sound!", SDL_GetError());
    }
}

sound::~sound() {
    SDL_free(m_data);
}

SDL_AudioSpec audio_engine::m_specs;
basic_stream audio_engine::m_stream;
std::map<std::string, std::unique_ptr<sound>> audio_engine::m_sounds;
std::vector<basic_stream> audio_engine::m_active_streams;

int audio_engine::frames_before_stream_check = 0;
int audio_engine::frames_for_stream_check = 60;

audio_engine::audio_engine()
{
    m_specs.format = SDL_AUDIO_F32;
    m_specs.channels = 2;
    m_specs.freq = 44100;

    m_stream = basic_stream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_specs, NULL, NULL);

    m_active_streams.reserve(16);
    for (int i = 0; i < m_active_streams.capacity(); i++) {
        m_active_streams.emplace_back(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_specs, nullptr, nullptr);
    }   
}

audio_engine::~audio_engine()
{
    unload_sounds();
}

void audio_engine::calc_frame_before_check() {
    frames_for_stream_check = 60 - m_active_streams.size();
    frames_for_stream_check = frames_for_stream_check < 1 ? 1 : frames_for_stream_check;
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
    m_active_streams.clear();
    m_sounds.clear();
    m_stream.~basic_stream();
}

sound* audio_engine::get_sound(std::string name)
{
    auto sound = m_sounds.find(name);
    if (sound != m_sounds.end()) {
        return (*sound).second.get();
    }
    return nullptr;
}

void audio_engine::play_sound(std::string name)
{
    auto sound = get_sound(name);
    if (sound) {
        if (sound->is_init()) {
            
            try_get_stream:
            auto free_stream = std::find_if(m_active_streams.begin(), m_active_streams.end(), [](basic_stream& stream) {
                return stream.is_ended();
            });

            if (free_stream == m_active_streams.end()) {
                m_active_streams.emplace_back(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_specs, nullptr, nullptr);
                calc_frame_before_check();
                goto try_get_stream;
            } else {
                auto stream = free_stream->get_stream();
                SDL_SetAudioStreamFormat(stream, sound->get_spec(), &m_specs);
                if (!SDL_PutAudioStreamData(stream, sound->get_data(), sound->get_size())) {
                    print::warning("Cant play audio!", name);
                    return;
                }
                SDL_FlushAudioStream(stream);
                SDL_ResumeAudioStreamDevice(stream);
            }
        }
    }
}

void audio_engine::check_active_streams()
{
    if (m_active_streams.size() > 16) {
        frames_before_stream_check++;
        if (frames_before_stream_check > frames_for_stream_check) {
            std::erase_if(m_active_streams, [](basic_stream& s){
                if (SDL_GetAudioStreamQueued(s.get_stream()) <= 0) {
                    return true;
                }
                return false;
            });
            frames_before_stream_check = 0;
        }
    }
}

basic_stream::basic_stream(SDL_AudioDeviceID devid, const SDL_AudioSpec *spec, SDL_AudioStreamCallback callback, void *userdata)
{
    m_stream = SDL_OpenAudioDeviceStream(devid, spec, callback, userdata);
    if (!m_stream) {
        print::error("Failed to open audio stream!", SDL_GetError());
    }
    if (!SDL_ResumeAudioStreamDevice(m_stream)) {
        print::error("Failed to resume audio stream!", SDL_GetError());
    }
    m_init = true;
}

basic_stream::~basic_stream() {
    if (m_stream) {
        SDL_DestroyAudioStream(m_stream);
        m_stream = nullptr;
    }

    m_init = false;
}
