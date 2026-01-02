#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <map>

#include "utils.h"
#include "game_object.h"
#include "texture.h"
#include "vector.h"

#define INDEV_ANIMATIONS_DISABLED


namespace animation {

#ifndef INDEV_ANIMATIONS_DISABLED
	struct transformation {
		vec2 pos = vec2(0.0f,0.0f);
		float angle = 0.0f;
	};

	enum class events {
		on_animation_start,
		on_animation_end,
	};

	class animation_type_base {
	protected:
		float len_ms = 0;

		with_default_value<transformation> delta_trans;

		std::map<animation::events, std::vector<std::function<void()>>> events;

		void reset_animation() {
			delta_trans.reset();
		}

		void do_event(animation::events event_type) {
			auto& events_ = events[event_type];

			for (auto& event : events_) {
				event();
			}
		}

		void base_on_animation_end()
		{
			this->~animation_type_base();
		}
	public:
		animation_type_base() {
			add_event(events::on_animation_end, [this] {base_on_animation_end(); });
		};
		animation_type_base(vec2 delta_pos, float angle, float time_ms)
			: animation_type_base()
		{
			this->delta_trans = { delta_pos, angle };
			len_ms = time_ms;
		}
		animation_type_base(float x, float y, float angle, float time_ms)
			: animation_type_base(vec2(x,y), angle, time_ms) {}
		virtual ~animation_type_base() = default;
		virtual transformation get(float delta_time, transformation obj_tr) = 0;

		animation_type_base& add_event(animation::events event_type, std::function<void()> func) {
			events[event_type].emplace_back(func);
			return *this;
		}
	};
	class linear : public animation_type_base {
	private:
		vec2 delta_pos, res_pos;
		float delta_angle, res_angle;
		double delta_time, current_time;
	public:
		linear(vec2 delta_pos, float angle, float time_ns);
		linear(float x, float y, float angle, float time_ns) : linear(vec2(x, y), angle, time_ns) {};
		~linear() = default;

		transformation get(float delta_time, transformation obj_tr) override;
	};
	class ease_in : public animation_type_base {
	public:

	};

	class physic_animation_player {
	private:
		std::vector<animation_type_base*> animations;

		void delete_animation(size_t id);
		bool check_if_buffer_full_of_nulls();
		void auto_clear();
	public:
		physic_animation_player() = default;
		~physic_animation_player();

		transformation update(float delta_time, transformation obj_tr);
		animation_type_base* get(size_t id);

		physic_animation_player& add(animation_type_base* anim_ptr);
	};

#endif

	class frame : public game_object {
		private:
			std::shared_ptr<texture> txt;
			std::string path; //for destructor logs
			int msec = 0;
			vec2 delta_pos;
		public:
			frame(SDL_Renderer* render, std::string path, int time);
			~frame() override;

			SDL_AppResult update(float delta_time) override;
			SDL_AppResult input(const SDL_Event* event) override;

			void set_pos(vec2 pos);
			void move_on(vec2 velocity);

			void set_size(vec2 size);

			void rotate(double angle);

			vec2 get_size();
			vec2 get_pos();

			float get_ratio();
			 
			void draw() override;
			bool check_collision(game_object* object) override;
			void clear_collision_buffer() override;

			std::shared_ptr<texture> get();
			int get_time();

			OBJECT::TYPE get_type() override {
				return OBJECT::TYPE::ANIMATION_FRAME;
			}
	};

	class cycle : public game_object {
		private:
			std::vector<std::shared_ptr<animation::frame>> frames;
			size_t current_frame = 0;

			int frames_passed = 0;
		public:
			cycle();
			~cycle() override;

			void add_frame(std::shared_ptr<animation::frame> frame);

			SDL_AppResult update(float delta_time) override;
			SDL_AppResult input(const SDL_Event* event) override;

			void set_pos(vec2 pos);
			void move_on(vec2 velocity);

			void set_size(vec2 size);

			void rotate(double angle);

			vec2 get_size();
			vec2 get_pos();

			float get_ratio();

			OBJECT::TYPE get_type() override {
				return OBJECT::TYPE::ANIMATION_CYCLE;
			};

			std::shared_ptr<animation::frame> get();

			void draw() override;
			bool check_collision(game_object* object) override;
			void clear_collision_buffer() override;

			void play(float fps);
	};

	class player : public game_object {
	private:
		std::map<std::string, std::shared_ptr<animation::cycle>> animations;
		std::string current_sprite_animation;
	public:
		player(SDL_Renderer* render, std::string animation_config);
		~player() override;

		SDL_AppResult update(float delta_time) override;
		SDL_AppResult input(const SDL_Event* event) override;

		void set_pos(vec2 pos);
		void move_on(vec2 velocity);

		void set_size(vec2 size);

		void rotate(double angle);

		vec2 get_size();
		vec2 get_pos();

		float get_ratio();

		std::shared_ptr<animation::cycle> get();

		void draw() override;
		bool check_collision(game_object* object) override;
		void clear_collision_buffer() override;

		void play(std::string name, float fps);

		OBJECT::TYPE get_type() override {
			return OBJECT::TYPE::ANIMATION_PLAYER;
		}
	};
}

