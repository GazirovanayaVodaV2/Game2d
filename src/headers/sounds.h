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

class audio_engine {
    private:
        static SDL_AudioSpec m_specs;
        static SDL_AudioStream* m_stream;

        static std::map<std::string, std::unique_ptr<sound>> m_sounds;

        static std::vector<SDL_AudioStream*> m_active_streams;

        static int frames_before_stream_check;

        audio_engine();
        ~audio_engine();
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
        static SDL_AudioStream* get_stream() {
            return m_stream;
        }
};