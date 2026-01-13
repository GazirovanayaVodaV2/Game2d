#pragma once
#include <base_object.h>
#include <texture.h>
#include <rgba.h>
#include <memory>
#include <vector>

#include <window.h>

namespace light {
	class source : public base_object
	{
		private:
			std::shared_ptr<texture> light_txt;
			float radius = 1.0f;
			rgba color = rgba(0xffffffff);
		protected:
			void draw_selection();
		public:
			source(std::string path_, float radius = 1.0f, hex color = 0xffffffff);
			~source() override;

			SDL_AppResult update(float delta_time) override;
			SDL_AppResult input(const SDL_Event* event) override;

			void set_pos(vec2 pos);
			void move_on(vec2 velocity);

			void set_size(vec2 size);

			void rotate(double angle);

			vec2 get_size();
			vec2 get_pos();

			float get_ratio();
			void draw();

			float get_radius();
			void set_radius(float radius);

			void set_color(int clr);

			OBJECT::TYPE get_type() override { return OBJECT::TYPE::LIGHT_SOURCE; }
	};

	class system : public base_object {
	private:
		std::vector<std::shared_ptr<light::source>> lights;

		SDL_Texture* dark = nullptr;

	public:
		system();
		~system() override;

		void add_light(std::string path, float radius = 1.0f, hex color = 0xffffffff);
		std::shared_ptr<light::source> get_light(size_t id);
		std::shared_ptr<light::source> get_last();
		size_t get_count();
		void clear();


		SDL_AppResult update(float delta_time) override;
		SDL_AppResult input(const SDL_Event* event) override;

		void set_pos(vec2 pos);
		void move_on(vec2 velocity);

		void set_size(vec2 size);

		void rotate(double angle);

		float get_ratio();

		vec2 get_size();
		vec2 get_pos();
		void draw();

		SDL_Texture* get_dark() { return dark; }

		OBJECT::TYPE get_type() override { return OBJECT::TYPE::LIGHT_SYSTEM; }
	};
}

