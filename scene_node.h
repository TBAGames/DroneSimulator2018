#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "resource.h"

namespace game {

    // Class that manages one object in a scene 
    class SceneNode {

        public:
            // Create scene node from given resources
            SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture = NULL);
			SceneNode(const std::string name);

            // Destructor
            ~SceneNode();
            
            // Get name of node
            const std::string GetName(void) const;

            // Draw the node according to scene parameters in 'camera'
            // variable
            //virtual void Draw(Camera *camera);
			virtual void SetupGeometry();
			virtual void DrawGeometry(bool toonModeOn, float roughness);

            // Update the node
            virtual void Update(void);

            // OpenGL variables
            GLenum GetMode(void) const;
            GLuint GetArrayBuffer(void) const;
            GLuint GetElementArrayBuffer(void) const;
            GLsizei GetSize(void) const;
            GLuint GetMaterial(void) const;

			// Perform transformations on node
			void Translate(glm::vec3 trans);
			void Rotate(glm::quat rot);
			void Scale(glm::vec3 scale);
		
			// Set node attributes
			void SetPosition(glm::vec3 position);
			void SetOrientation(glm::quat orientation);
			void SetScale(glm::vec3 scale);

			// Get node attributes
			glm::vec3 GetPosition(void) const;
			glm::quat GetOrientation(void) const;
			glm::vec3 GetScale(void) const;

			// Rotate vectors with quaternions
			glm::vec3 qrot(glm::quat q, glm::vec3 v);

			// Get relative attributes of camera
			glm::vec3 GetForward(void) const;
			glm::vec3 GetSide(void) const;
			glm::vec3 GetUp(void) const;

			// Perform relative transformations of camera
			void Pitch(float angle);
			void Yaw(float angle);
			void Roll(float angle);

			// Manage hierarchy
			std::vector<SceneNode *> GetChildren();
			SceneNode *GetParent();
			void AddChild(SceneNode * child);
			void SetParent(SceneNode * parent);
			void RemoveChild(SceneNode * node);
			void RemoveParent();
			SceneNode *GetChild(std::string name);

			// Recursively grab the subtree with root this
			std::vector<SceneNode *> *BuildNodeSubTree(std::vector<SceneNode *> *buildTree);

		protected:
			glm::vec3 position_; // Position of node
			glm::quat orientation_; // Orientation of node
			glm::vec3 scale_; // Scale of node

			glm::vec3 forward_ = glm::vec3(0.0, 0.0, -1.0); // Initial forward vector
			glm::vec3 side_ = glm::vec3(1.0, 0.0, 0.0); // Initial side vector

			SceneNode *parent_;
			std::vector<SceneNode *> children_;

        private:
            std::string name_; // Name of the scene node
            GLuint array_buffer_; // References to geometry: vertex and array buffers
            GLuint element_array_buffer_;
            GLenum mode_; // Type of geometry
            GLsizei size_; // Number of primitives in geometry
            GLuint material_; // Reference to shader program
            GLuint texture_; // Reference to texture resource

            // Set matrices that transform the node in a shader program
            void SetupShader(GLuint program, bool toonModeOn, float roughness);

    }; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
