#ifndef GAME_H_
#define GAME_H_

#include <exception>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene_graph.h"
#include "resource_manager.h"
#include "camera.h"
#include "asteroid.h"
#include "settings.h"
#include "Bullet.h"
#include "Bomb.h"
#include "Rocket.h"

namespace game {

    // Exception type for the game
    class GameException: public std::exception
    {
        private:
            std::string message_;
        public:
            GameException(std::string message) : message_(message) {};
            virtual const char* what() const throw() { return message_.c_str(); };
            virtual ~GameException() throw() {};
    };

    // Game application
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();
            // Call Init() before calling any other method
            void Init(void); 
            // Set up resources for the game
            void SetupResources(void);
            // Set up initial scene
            void SetupScene(void);
            // Run the game: keep the application active
            void MainLoop(void); 

			// Settings-specific shader funcs
			void ToggleToonMode(void);
			void IncrementSurfaceRoughness(void);
			void DecrementSurfaceRoughness(void);

			// Get camera
			Camera *GetCamera(void);

			// Update non-camera objects
			void GameObjectUpdate(void);

			
			

        private:
            // GLFW window
            GLFWwindow* window_;

            // Scene graph containing all nodes to render
            SceneGraph scene_;

            // Resources available to the game
            ResourceManager resman_;

            // Camera abstraction
            Camera camera_;

            // Flag to turn animation on/off
            bool animating_;

            // Methods to initialize the game
            void InitWindow(void);
            void InitView(void);
            void InitEventHandlers(void);
			//void InitSettings(void);
 
            // Methods to handle events
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void ResizeCallback(GLFWwindow* window, int width, int height);
			SceneNode *skybox_;

            // Asteroid field
            // Create instance of one asteroid
            Asteroid *CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name);
            // Create entire random asteroid field
            void CreateAsteroidField(int num_asteroids = 1500);

            // Create an instance of an object stored in the resource manager
            SceneNode *CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));

			
			//Shooting the machine gun
			void FireMachineGun(void);
			Bullet *CreateBullet(void);

			//Dropping the bomb 
			void DropBomb(void);
			Bomb *CreateBomb(void);

			//Firing the rockets
			void EngageRockets(void);
			Rocket *CreateRocket(void);

			// Settings
			Settings settings_;
			bool IsToonModeOn(void);
			float GetRoughness(void);

			// Handle collisions between gameobjects
			void CheckCollisions(void);
			bool IsCollidingBuilding(SceneNode *ship, SceneNode *building);

			// Win con
			void CheckWin(void);
			
    }; // class Game

} // namespace game

#endif // GAME_H_
