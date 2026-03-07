#pragma once
#include "SDL3/SDL_audio.h"
#include <string>
#include <cstdint>
#include <map>
#include <vector>
#include <memory>

class sound {
private:
    uint8_t* m_data = NULL;
    uint32_t m_size = 0;
    SDL_AudioSpec m_filespec;
    std::string m_path;
public:
    sound() = default;
    sound(std::string path);
    ~sound();

    SDL_AudioSpec* get_spec() {
        return &m_filespec;
    }

    bool is_init() {
        return m_size > 0 && m_data;
    }

    uint8_t* get_data() {
        return m_data;
    }
    uint32_t get_size() {
        return m_size;
    }
};

class basic_stream {
    private:
        SDL_AudioStream* m_stream = nullptr;
        bool m_init = false;
    public:
        basic_stream() = default;
        basic_stream(SDL_AudioDeviceID devid, const SDL_AudioSpec *spec, SDL_AudioStreamCallback callback, void *userdata);
        ~basic_stream();

        basic_stream(const basic_stream&) = delete;
        basic_stream& operator=(const basic_stream&) = delete;

        basic_stream(basic_stream&& other) noexcept 
            : m_stream(other.m_stream), m_init(other.m_init) {
            other.m_stream = nullptr;
            other.m_init = false;
        }

        basic_stream& operator=(basic_stream&& other) noexcept {
            if (this != &other) {
                if (m_stream) SDL_DestroyAudioStream(m_stream);
                m_stream = other.m_stream;
                m_init = other.m_init;
                other.m_stream = nullptr;
                other.m_init = false;
            }
            return *this;
        }

        SDL_AudioStream* get_stream() {
            return m_stream;
        }
        bool is_init() {
            return m_init;
        }

        bool is_ended() {
            return SDL_GetAudioStreamQueued(m_stream) <= 0;
        }
};

class audio_engine {
    private:
        static SDL_AudioSpec m_specs;
        static basic_stream m_stream;

        static std::map<std::string, std::unique_ptr<sound>> m_sounds;

        static std::vector<basic_stream> m_active_streams;

        static int frames_before_stream_check, frames_for_stream_check;

        audio_engine();
        ~audio_engine();

        static void calc_frame_before_check();
    public:
        static audio_engine& get_instance() {
            static audio_engine self;
            return self;
        }

        static void load_sounds();
        static void unload_sounds();
        static sound* get_sound(std::string name);

        static void play_sound(std::string name);

        static void check_active_streams();

        static SDL_AudioSpec* get_specs() {
            return &m_specs;
        }
        static basic_stream& get_stream() {
            return m_stream;
        }
};