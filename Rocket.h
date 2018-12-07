#pragma once
#ifndef ROCKET_H_
#define ROCKET_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"

namespace game {

	// Abstraction of a Rocket
	class Rocket : public SceneNode {

	public:
		// Create Rocket from given resources
		Rocket(const std::string name, const Resource *geometry, const Resource *material);

		// Destructor
		~Rocket();

		void SetDir(glm::vec3 direction);

		// Update geometry configuration
		void Update(void);

	private:
		glm::vec3 direction_;
	};

} // namespace game

#endif 