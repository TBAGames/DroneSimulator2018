#include "Bomb.h"

namespace game {

	Bomb::Bomb(const std::string name, const Resource *geometry, const Resource *material) : SceneNode(name, geometry, material) {
	}


	Bomb::~Bomb() {
	}

	void Bomb::SetDir(glm::vec3 direction)
	{
		direction_ = direction;
	}

	void Bomb::DestroyProjectile(bool condition)
	{
		if (condition)
		{
			delete this;
		}
	}

	void Bomb::Update(void) {

		glm::vec3 pos = GetPosition();
		Translate(direction_ * speed_);

	}

} // namespace game