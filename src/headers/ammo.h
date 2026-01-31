#include "interactive_object.h"

class ammo : public interactive_object_base {
private:
	int m_ammo_type = 762;
	int m_count = 20;

	texture_from_atlas m_texture = nullptr;

	void interaction_impl(entity* ent) override {};
	bool use_impl(entity* ent) override { return false; };
public:
	bool is_pickable() {
		return true;
	}

	ammo(texture_from_atlas txt, int ammo_type, int count);
	~ammo() = default;

	void draw();
	SDL_AppResult update(float delta);

	int get_count() {
		return m_count;
	}
	void set_count(int val) {
		m_count = val;
	}

	virtual std::string get_name() { return "ammo"; };
};