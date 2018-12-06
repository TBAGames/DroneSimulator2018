#include "Bullet.h"

namespace game {

	Bullet::Bullet(const std::string name, const Resource *geometry, const Resource *material) : SceneNode(name, geometry, material) {
	}


	Bullet::~Bullet() {
	}

	void Bullet::SetDir(glm::vec3 direction)
	{
		direction_ = direction;
	}

	void Bullet::Update(void) {

		glm::vec3 pos = GetPosition();
		Translate(direction_ * speed_);
	

	}

} // namespace game