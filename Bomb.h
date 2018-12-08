#pragma once
#ifndef BOMB_H_
#define BOMB_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"
#include "Projectile.h"

namespace game {

	// Abstraction of a bullet
	class Bomb : public SceneNode, public Projectile {

	public:
		// Create bomb from given resources
		Bomb(const std::string name, const Resource *geometry, const Resource *material);

		// Destructor
		~Bomb();

		void SetDir(glm::vec3 direction);

		void DestroyProjectile(bool condition);

		// Update geometry configuration
		void Update(void);

	private:
		glm::vec3 direction_;
		//const float speed_ = 9.8f;
	};

} // namespace game

#endif 