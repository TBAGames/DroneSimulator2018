#ifndef CAMERA_H_
#define CAMERA_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "scene_node.h"

namespace game {

    // Abstraction of a camera
    class Camera : public SceneNode {

		enum class CameraMode { FIRST_PERSON, THIRD_PERSON };

        public:
            Camera(void);
            ~Camera();
 
            // Get global camera attributes
            //glm::vec3 GetPosition(void) const;
            //glm::quat GetOrientation(void) const;

            // Set global camera attributes
            //void SetPosition(glm::vec3 position);
            //void SetOrientation(glm::quat orientation);
            
            // Perform global transformations of camera
            //void Translate(glm::vec3 trans);
            //void Rotate(glm::quat rot);

            // Set the view from camera parameters: initial position of camera,
            // point looking at, and up vector
            // Resets the current orientation and position of the camera
            void SetView(glm::vec3 position, glm::vec3 look_at, glm::vec3 up);
            // Set projection from frustum parameters: field-of-view,
            // near and far planes, and width and height of viewport
            void SetProjection(GLfloat fov, GLfloat near, GLfloat far, GLfloat w, GLfloat h);
            // Set all camera-related variables in shader program
            void SetupShader(GLuint program);

			// Swwitch between first and third person
			void SwitchCameraMode(void);

			void Update(void) override;


        private:
            //glm::vec3 position_; // Position of camera
            //glm::quat orientation_; // Orientation of camera
            glm::mat4 view_matrix_; // View matrix
            glm::mat4 projection_matrix_; // Projection matrix

            // Create view matrix from current camera parameters
            void SetupViewMatrix(void);

			CameraMode camera_mode_;

    }; // class Camera

} // namespace game

#endif // CAMERA_H_
