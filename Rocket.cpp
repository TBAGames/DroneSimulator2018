#include "Rocket.h"

namespace game {

	Rocket::Rocket(const std::string name, const Resource *geometry, const Resource *material) : SceneNode(name, geometry, material) {
	}


	Rocket::~Rocket() {
	}

	void Rocket::SetDir(glm::vec3 direction)
	{
		direction_ = direction;
	}
	void Rocket::DestroyProjectile(bool condition) 
	{
		if (condition) 
		{
			delete this;
		}
	}

	void Rocket::Update(void) {
		Translate(direction_ * speed_);
	}

} // namespace game