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
			static inline const OBJECT::TYPE type = OBJECT::TYPE::LIGHT_SOURCE;

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

			OBJECT::TYPE get_type() override;

			float get_radius();
			void set_radius(float radius);
	};

	class system : public base_object {
	private:
		std::vector<std::shared_ptr<light::source>> lights;

		SDL_Texture* dark = nullptr;

	protected:
		static inline const OBJECT::TYPE type = OBJECT::TYPE::LIGHT_SYSTEM;

	public:
		system();
		~system() override;

		void add_light(std::string path, float radius = 1.0f, hex color = 0xffffffff);
		std::shared_ptr<light::source> get_light(size_t id);
		std::shared_ptr<light::source> get_last();
		size_t get_count();

		SDL_AppResult update(float delta_time) override;
		SDL_AppResult input(const SDL_Event* event) override;

		void set_pos(vec2 pos) override;
		void move_on(vec2 velocity) override;

		void set_size(vec2 size) override;

		void rotate(double angle) override;

		vec2 get_size() override;
		vec2 get_pos() override;

		float get_ratio() override;
		void draw();

		OBJECT::TYPE get_type() override;
	};
}

