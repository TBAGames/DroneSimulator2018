#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "bin/path_config.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;
const float rotation_factor = glm::pi<float>() / 360;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 25.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.6, 0.0, 0.0);
int rotation_degree_pitch = 0;
int rotation_degree_yaw = 0;
int rotation_degree_roll = 0;
int movement_degree_fwd = 0;
int movement_degree_up = 0;
glm::vec3 camera_position_g(0.5, 0.5, 10.0);
glm::vec3 camera_look_at_g(0.0, 0.0, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);
bool camswitch = false;
int camview = 0;
bool win_ = false;

//Counter to limit the bombs dropped
int counterB = 0;

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;


Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();
	//InitSettings();

    // Set variables
    animating_ = true;
}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
    }
    if (!window_){
        glfwTerminate();
        throw(GameException(std::string("Could not create window")));
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        throw(GameException(std::string("Could not initialize the GLEW library: ")+std::string((const char *) glewGetErrorString(err))));
    }
}


void Game::InitView(void){

    // Set up z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Set up camera
    // Set current view
	camera_.SwitchCameraMode();
    camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){

    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}

/*void Game::InitSettings(void) {
	settings_ = Globals();
}*/


void Game::SetupResources(void){

	//Create Sphere
	resman_.CreateSphere("SimpleSphereMesh", 0.75, 10, 10);
  
	std::string filename;

  // Create turret parts
  resman_.CreateCylinder("TurretMesh");

	// Create torus
	resman_.CreateTorus("TorusMesh");

	// Load material to be applied to particles
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/smoke");
	resman_.LoadResource(Material, "SmokeParticleMaterial", filename.c_str());

	// Load material to be applied to particles
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/muzzle_flash");
	resman_.LoadResource(Material, "MuzzleParticleMaterial", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/flame");
	resman_.LoadResource(Material, "FireParticleMaterial", filename.c_str());

	// Create particles for explosion
	resman_.CreateSphereParticles("SphereParticles");

  // Load material to be applied to turret
  filename = std::string(MATERIAL_DIRECTORY) + std::string("/metal");
  resman_.LoadResource(Material, "ShinyBlueMaterial", filename.c_str());

  // Load a cube from an obj file
  filename = std::string(MATERIAL_DIRECTORY) + std::string("/cube.obj");
  resman_.LoadResource(Mesh, "CubeMesh", filename.c_str());

  // Load texture to be applied to the cube
  filename = std::string(MATERIAL_DIRECTORY) + std::string("/checker.png");
  resman_.LoadResource(Texture, "Checker", filename.c_str());

  // Load material to be applied to the cube
  filename = std::string(MATERIAL_DIRECTORY) + std::string("/textured_material");
	resman_.LoadResource(Material, "TexturedMaterial", filename.c_str());

	// Load material to be applied to the cube
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/plastic");
	resman_.LoadResource(Material, "PlasticMaterial", filename.c_str());

	// Load building texture
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/building_texture.jpg");
	resman_.LoadResource(Texture, "BuildingTexture", filename.c_str());

	// Load textures to be applied to the cylinder
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/crystal.jpg");
	resman_.LoadResource(Texture, "Crystal", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/nebula.jpg");
	resman_.LoadResource(Texture, "Nebula", filename.c_str());

	// Create a simple sphere to represent the asteroids
	resman_.CreateSphere("SimpleSphereMesh", 0.5, 10, 10);

	// Load material to be applied to asteroids
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
	resman_.LoadResource(Material, "ObjectMaterial", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/asphalt.png");
	resman_.LoadResource(Texture, "Asphalt", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/end.png");
	resman_.LoadResource(Texture, "End", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/smoke.png");
	resman_.LoadResource(Texture, "Smoke", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/fire.png");
	resman_.LoadResource(Texture, "Fire", filename.c_str());

	// Load cube map to be applied to skybox
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/island/island.tga");
	resman_.LoadResource(CubeMap, "LakeCubeMap", filename.c_str());

	// Load material to be applied to skybox
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/skybox");
	resman_.LoadResource(Material, "SkyboxMaterial", filename.c_str());
}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

	// Create Ship
	SceneNode *ship = CreateAsteroidInstance("Ship", "SimpleSphereMesh", "ShinyBlueMaterial");
	SceneNode *arms1 = CreateInstance("ShipArm1", "CubeMesh", "ShinyBlueMaterial");
	SceneNode *arms2 = CreateInstance("ShipArm2", "CubeMesh", "ShinyBlueMaterial");
	SceneNode *propel1 = CreateInstance("Pro1", "CubeMesh", "ShinyBlueMaterial");
	SceneNode *propel2 = CreateInstance("Pro2", "CubeMesh", "ShinyBlueMaterial");
	SceneNode *propel3 = CreateInstance("Pro3", "CubeMesh", "ShinyBlueMaterial");
	SceneNode *propel4 = CreateInstance("Pro4", "CubeMesh", "ShinyBlueMaterial");

	camera_.SwitchCameraMode();

	ship->AddChild(arms1);
	ship->AddChild(arms2);
	arms1->AddChild(propel1);
	arms1->AddChild(propel2);
	arms2->AddChild(propel3);
	arms2->AddChild(propel4);

	//Propeller Position
	propel1->Translate(glm::vec3(-1.75, -0.15, 0.0));
	propel2->Translate(glm::vec3(1.75, -0.15, 0.0));
	propel3->Translate(glm::vec3(0.0, -0.15, -1.75));
	propel4->Translate(glm::vec3(0.0, -0.15, 1.75));

	//Ship Arms Scaling
	arms1->Scale(glm::vec3(2.0, 0.1, 0.25));
	arms2->Scale(glm::vec3(0.25, 0.1, 2.0));

	//Propeller Scaling
	propel1->Scale(glm::vec3(0.5, 0.05, 0.1));
	propel2->Scale(glm::vec3(0.5, 0.05, 0.1));
	propel3->Scale(glm::vec3(0.1, 0.05, 0.5));
	propel4->Scale(glm::vec3(0.1, 0.05, 0.5));

    // Adjust the instance
	game::SceneNode *projectileContainer = CreateInstance("Projectiles", "CubeMesh", "ShinyBlueMaterial");
	projectileContainer->SetScale(0.001f*glm::vec3(1.0, 1.0, 1.0));

	// Create ground
	SceneNode *ground = CreateInstance("Ground", "CubeMesh", "TexturedMaterial", "Asphalt");
	ground->SetPosition(ship->GetPosition() - glm::vec3(0.0, -50.0, 0.0));
	ground->SetOrientation(ship->GetOrientation());
	ground->SetScale(glm::vec3(500.0f, 0.1f, 500.0f));

	skybox_ = CreateInstance("CubeInstance1", "CubeMesh", "SkyboxMaterial");
	skybox_->Scale(glm::vec3(50.0, 50.0, 50.0));
	skybox_->Roll(glm::pi<float>());

	// Create Buildings
	int numBuildings = 8;
	for (int i = 0; i < numBuildings; i++)
	{
		for (int j = 0; j < numBuildings; j++)
		{
			SceneNode *building = CreateInstance("Building" + (i*numBuildings) + j, "CubeMesh", "TexturedMaterial", "BuildingTexture");
			ground->AddChild(building);
			building->SetScale(glm::vec3(5.0, 100.0, 5.0));
			building->SetPosition(glm::vec3(10.0 + 100.0*(i - (int)(numBuildings / 2)), 50.0, 100.0*(j - (int)(numBuildings / 2))));
		}

		int numEnemies = 3;
		game::SceneNode *enemyContainer = CreateInstance("Enemies", "CubeMesh", "ShinyBlueMaterial");
		for (int i = 0; i < numEnemies; i++)
		{
			for (int j = 0; j < numEnemies; j++)
			{
				if (j % 2 == 0)
				{
					Enemy *enemy = CreateEnemyInstance("Enemy" + (i*numEnemies) + j, "CubeMesh", "ShinyBlueMaterial", "BuildingTexture", glm::vec3(100.0*(i - (int)(numEnemies / 2)) + 25, 0.0, 100.0*(j - (int)(numEnemies / 2)) + 25), ship, "Bird");
					enemyContainer->AddChild(enemy);
				}
				else
				{
					Enemy *enemy = CreateEnemyInstance("Enemy" + (i*numEnemies) + j, "CubeMesh", "ShinyBlueMaterial", "BuildingTexture", glm::vec3(100.0*(i - (int)(numEnemies / 2)) + 25, 50.0f, 100.0*(j - (int)(numEnemies / 2)) + 25), ship, "Dog");
					enemyContainer->AddChild(enemy);
				}
			}
		}

		// Create endgoal
		SceneNode *end = CreateInstance("End", "CubeMesh", "TexturedMaterial", "End");
		end->SetPosition(ground->GetPosition() + glm::vec3(1.0, 0.0, 0.0)*ground->GetScale().x);
	}
}


bool Game::IsToonModeOn() {

	//std::cout << settings_.IsToonModeOn() << std::endl;
	return settings_.IsToonModeOn();
}


void Game::ToggleToonMode() {

	settings_.ToggleToonMode();
}


void Game::IncrementSurfaceRoughness() {

	settings_.IncrementSurfaceRoughness();
}

void Game::DecrementSurfaceRoughness() {
	
	settings_.DecrementSurfaceRoughness();
}

float Game::GetRoughness() {
	
	//std::cout << settings_.GetRoughness() << std::endl;
	return settings_.GetRoughness();
}


void Game::MainLoop(void){

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        // Animate the scene
        if (animating_ && !win_){
              static double last_time = 0;
              double current_time = glfwGetTime();
              if ((current_time - last_time) > 0.01){

                          // Animate the turret
                  SceneNode *node;
                  glm::quat rotation;

                          // Animate the ship
                          node = scene_.GetNode("Ship");
                  node->Pitch(rotation_factor*rotation_degree_pitch);
                  node->Yaw(rotation_factor*rotation_degree_yaw);
                  node->Roll(rotation_factor*rotation_degree_roll);
                  node->Translate((node->GetForward()/100.0f) * float(movement_degree_fwd));
                  node->Translate((node->GetUp() / 100.0f) * float(movement_degree_up));

                  camera_.Update();
                  GameObjectUpdate();

                  SceneNode * proj = scene_.GetNode("Projectiles");
                  for (int i = 0; i < proj->GetChildren().size(); i++)
                  {
                    SceneNode * projNode = scene_.GetNode("Projectiles")->GetChildren()[i];
                    if (glm::distance(node->GetPosition(), scene_.GetNode("Projectiles")->GetChildren()[i]->GetPosition()) > 100.0f) 
                    {
                      scene_.GetNode("Projectiles")->RemoveChild(projNode);
                      scene_.RemoveNode(projNode);
                    }
                  }


				  SceneNode * enemyContainer = scene_.GetNode("Enemies");
				  for (int i = 0; i < enemyContainer->GetChildren().size(); i++)
				  {
					  Enemy * enemy = (Enemy*)enemyContainer->GetChildren()[i];
					  enemy->beHAVE();
				  }
                
                
                  // Rotate propeller blades
                  std::vector<SceneNode *> children = node->GetChildren();
                  for (std::vector<SceneNode *>::iterator ptr = children.begin(); ptr < children.end(); ptr++) {
                    if ((*ptr)->GetName().find("arms")) {
                      std::vector<SceneNode *> a_children = (*ptr)->GetChildren();
                      for (std::vector<SceneNode *>::iterator ptr2 = a_children.begin(); ptr2 < a_children.end(); ptr2++) {							
                        (*ptr2)->Yaw(30.0);
                        (*ptr2)->Update();
                      }
                    }
                  }

				  CheckWin();

                  CheckCollisions();
                  skybox_->SetPosition(node->GetPosition());
                
                  last_time = current_time;

              }
        }

        // Draw the scene
        scene_.Draw(&camera_, IsToonModeOn(), GetRoughness());

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}

void Game::CheckWin() {
	SceneNode *ship = scene_.GetNode("Ship");
	SceneNode *end = scene_.GetNode("End");

	if (glm::distance(ship->GetPosition(), end->GetPosition()) < 5.0f)
	{
		win_ = true;
		std::cout << "WIN!" << std::endl;
	}
}

void Game::CheckCollisions(void)
{
	SceneNode *ship = scene_.GetNode("Ship");
	std::vector<SceneNode *> buildings = scene_.GetNode("Ground")->GetChildren();
	//std::vector<SceneNode *>::const_iterator i;

	for (std::vector<SceneNode *>::const_iterator ptr = buildings.begin(); ptr < buildings.end(); ptr++)
	{
		if (IsCollidingBuilding(ship, *ptr))
		{
			std::cout << "Colliding" << std::endl;
		}
	}
}

bool Game::IsCollidingBuilding(SceneNode * ship, SceneNode *building)
{
	glm::vec3 s = ship->GetPosition();
	glm::vec3 b = building->GetPosition();
	glm::vec3 bScale = building->GetScale();

	float bXMax = b.x + bScale.x;
	float bXMin = b.x - bScale.x;
	float bYMax = b.y + bScale.y;
	float bYMin = b.y - bScale.y;
	float bZMax = b.z + bScale.z;
	float bZMin = b.z - bScale.z;

	if (s.x > bXMin && s.x < bXMax &&
		s.y > bYMin && s.y < bYMax &&
		s.z > bZMin && s.z < bZMax)
	{
		return true;
	}

	return false;
}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    // Get user data with a pointer to the game class
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;
	

    // Quit game if 'q' is pressed
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    // Stop animation if space bar is pressed
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        game->animating_ = (game->animating_ == true) ? false : true;
    }

	//Fire the turret
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		game->FireMachineGun();
	}

	//Drop the bombs, it it isn't depleted
	if (counterB < 3)
	{
		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
			game->DropBomb();
			counterB++;
			std::cout <<"Bombs Left: " << 3-counterB << std::endl;

		}
	}

	//Launch the rockets
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		game->EngageRockets();
	}
	

    // View control
    float rot_factor(glm::pi<float>() / 180);
    float trans_factor = 1.0;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
        //game->camera_.Pitch(rot_factor);
		rotation_degree_pitch += 1;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        //game->camera_.Pitch(-rot_factor);
		rotation_degree_pitch -= 1;
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        //game->camera_.Yaw(rot_factor);
		rotation_degree_yaw += 1;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        //game->camera_.Yaw(-rot_factor);
		rotation_degree_yaw -= 1;
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS){
        //game->camera_.Roll(rot_factor);
		rotation_degree_roll += 1;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS){
       // game->camera_.Translate(-game->camera_.GetForward()*trans_factor);
		rotation_degree_roll -= 1;
    }
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		//game->camera_.Roll(-rot_factor);
		movement_degree_fwd += 1;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		//game->camera_.Translate(game->camera_.GetForward()*trans_factor);
		movement_degree_fwd -= 1;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		movement_degree_up += 1;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		movement_degree_up -= 1;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		camswitch = true;
	}
    if (key == GLFW_KEY_J){
        game->camera_.Translate(-game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_L){
        game->camera_.Translate(game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_I){
        game->camera_.Translate(game->camera_.GetUp()*trans_factor);
    }
    if (key == GLFW_KEY_K){
        game->camera_.Translate(-game->camera_.GetUp()*trans_factor);
    }
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		game->ToggleToonMode();
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		game->IncrementSurfaceRoughness();
	}
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		game->DecrementSurfaceRoughness();
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		game->GetCamera()->SwitchCameraMode();
	}
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height){

    // Set up viewport and camera projection based on new window size
    glViewport(0, 0, width, height);
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;
    game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


Game::~Game(){
    
    glfwTerminate();
}


Asteroid *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name){

    // Get resources
    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    // Create asteroid instance
    Asteroid *ast = new Asteroid(entity_name, geom, mat);
	scene_.AddNode(ast);
    camera_.AddChild(ast);
    return ast;
}


void Game::CreateAsteroidField(int num_asteroids){

    // Create a number of asteroid instances
    for (int i = 0; i < num_asteroids; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;

        // Create asteroid instance
        Asteroid *ast = CreateAsteroidInstance(name, "SimpleSphereMesh", "ShinyBlueMaterial");

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum
        ast->SetPosition(glm::vec3(-300.0 + 600.0*((float) rand() / RAND_MAX), -300.0 + 600.0*((float) rand() / RAND_MAX), 600.0*((float) rand() / RAND_MAX)));
        ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
        ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
    }
}



SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name){

    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    Resource *tex = NULL;
    if (texture_name != ""){
        tex = resman_.GetResource(texture_name);
        if (!mat){
            throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
        }
    }

    SceneNode *scn = scene_.CreateNode(entity_name, geom, mat, tex);

	//By default, add new node as a child of scene
	scene_.AddNode(scn);
    return scn;
}

Camera *Game::GetCamera() {
	return &camera_;
}

Enemy *Game::CreateEnemyInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name, glm::vec3 origin_point, SceneNode *player, std::string type) {

	// Get resources
	Resource *geom = resman_.GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	Resource *tex = NULL;
	if (texture_name != "") {
		tex = resman_.GetResource(texture_name);
		if (!mat) {
			throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
		}
	}

	Enemy *ast = new Enemy(entity_name, geom, mat, tex, origin_point, player, type);
	scene_.AddNode(ast);
	camera_.AddChild(ast);
	return ast;
}
 
void Game::FireMachineGun()
{
	//Create an instance of the bullet 
	Bullet *bullet = CreateBullet();
	SceneNode *node = scene_.GetNode("Ship");
	SceneNode *projectileContainer = scene_.GetNode("Projectiles");
	SceneNode *bulletEffect = CreateInstance("ParticleInstance1", "SphereParticles", "MuzzleParticleMaterial");

	projectileContainer->AddChild(bullet);

	//Gets the position of where the bullet will start, the direction it will head, speed of the bullet
	bullet->SetPosition(node->GetPosition() + node->GetForward()*3.0f);
	bullet->SetOrientation(node->GetOrientation());
	bullet->SetDir(node->GetForward());
	bullet->SetSpeed(2.5f);

	bullet->AddChild(bulletEffect);

	bulletEffect->SetPosition(bullet->GetPosition() + -1.0f * bullet->GetForward());
	bulletEffect->SetOrientation(bullet->GetOrientation());

}

void Game::DropBomb()
{
	//Create an instance of the bomb 
	Bomb *bomb = CreateBomb();
	SceneNode *node = scene_.GetNode("Ship");
	SceneNode *projectileContainer = scene_.GetNode("Projectiles");
	SceneNode *bombEffect = CreateInstance("ParticleInstance1", "SphereParticles", "SmokeParticleMaterial", "Smoke");

	projectileContainer->AddChild(bomb);

	//Gets the position of where the bomb will start, the direction it will head, speed of the bomb
	bomb->SetPosition((node->GetPosition()) + node->GetUp()*-1.0f);
	bomb->SetOrientation(node->GetOrientation());
	bomb->SetDir(-(node->GetUp()));
	bomb->SetSpeed(0.25f);

	bomb->AddChild(bombEffect);

	bombEffect->SetPosition(bomb->GetPosition() + -3.0f * bomb->GetForward());
	bombEffect->SetOrientation(bomb->GetOrientation());

}

void Game::EngageRockets()
{
	//Create an instance of the rocket 
	Bomb *rocket = CreateBomb();
	SceneNode *node = scene_.GetNode("Ship");
	SceneNode *projectileContainer = scene_.GetNode("Projectiles");
	SceneNode *rocketEffect = CreateInstance("ParticleInstance1", "SphereParticles", "FireParticleMaterial", "Fire");

	projectileContainer->AddChild(rocket);

	//Gets the position of where the rocket will start, the direction it will head, speed of the rocket
	rocket->SetPosition((node->GetPosition()) + node->GetForward()*3.0f);
	rocket->SetOrientation(node->GetOrientation());
	rocket->SetDir(node->GetForward());
	rocket->SetSpeed(0.25f);

	rocket->AddChild(rocketEffect);

	rocketEffect->SetPosition(rocket->GetPosition() + -1.0f * rocket->GetForward());
	rocketEffect->SetOrientation(rocket->GetOrientation() + glm::quat(rocket->GetUp(), glm::vec3(3.5, 5.0, 0.0)));
}


Bullet *Game::CreateBullet()
{
	std::string entity_name = "Bullet";
	std::string object_name = "SimpleSphereMesh";
	std::string material_name = "ShinyBlueMaterial";

	Resource *geom = resman_.GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	// Create ship instance
	Bullet *bullet = new Bullet(entity_name, geom, mat);
	//scene_.AddNode(ast);

	//std::cout << "Adding laser" << std::endl;
	scene_.AddNode(bullet);

	return bullet;
}

Bomb *Game::CreateBomb()
{
	std::string entity_name = "Bomb";
	std::string object_name = "CubeMesh";
	std::string material_name = "ShinyBlueMaterial";

	Resource *geom = resman_.GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	// Create ship instance
	Bomb *bomb = new Bomb(entity_name, geom, mat);
	//scene_.AddNode(ast);

	//std::cout << "Adding laser" << std::endl;
	scene_.AddNode(bomb);

	return bomb;
}

Rocket *Game::CreateRocket()
{
	std::string entity_name = "Rocket";
	std::string object_name = "SimpleSphereMesh";
	std::string material_name = "ShinyBlueMaterial";

	Resource *geom = resman_.GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_.GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	// Create ship instance
	Rocket *rocket = new Rocket(entity_name, geom, mat);
	//scene_.AddNode(ast);

	//std::cout << "Adding laser" << std::endl;
	scene_.AddNode(rocket);

	return rocket;
}

void Game::GameObjectUpdate(void) {

	scene_.GetNode("Projectiles")->Update();
}

} // namespace game
