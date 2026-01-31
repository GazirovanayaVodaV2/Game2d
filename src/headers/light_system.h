#pragma once
#include "base_object.h"
#include <rgba.h>
#include <memory>
#include <vector>

#include "window.h"
#include "texture.h"
namespace light {
	class source : public base_object
	{
		private:
			std::unique_ptr<texture> light_txt;
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

			void rotate(double angle) {};

			vec2 get_size();
			vec2 get_pos();

			float get_ratio() { return 1.0f; };
			void draw();

			float get_radius();
			void set_radius(float radius);

			void set_color(int clr);
	};

	class system : public base_object {
	private:
		std::vector<std::shared_ptr<light::source>> lights;

		std::shared_ptr<texture> ambient = nullptr;
		rgba ambient_color;
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

		void set_size(vec2 size) {};

		void rotate(double angle) {};

		float get_ratio() {return 1.0f;};

		vec2 get_size() { return vec2(); };
		vec2 get_pos() { return vec2(); };
		void draw();

		void set_ambient(rgba clr);
		std::shared_ptr<texture> get_ambient() { return ambient; }
	};
}

