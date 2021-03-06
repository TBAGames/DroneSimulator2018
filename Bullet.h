#pragma once
#ifndef BULLET_H_
#define BULLET_H_

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
	class Bullet : public SceneNode, public Projectile {

	public:
		// Create bullet from given resources
		Bullet(const std::string name, const Resource *geometry, const Resource *material);

		// Destructor
		~Bullet();

		void SetDir(glm::vec3 direction);
		void DestroyProjectile(bool condition);

		// Update geometry configuration
		void Update(void);

	private:
		glm::vec3 direction_;
	}; 

} // namespace game

#endif 