#ifndef BIRD_H_
#define BIRD_H_

#include "scene_node.h"

class bird : public game::SceneNode 
{
	public:
		bird(const std::string name, const game::Resource *geometry, const game::Resource *material, const game::Resource *texture, glm::vec3 originPoint, SceneNode *play);
		glm::vec3 getOrigin(void);
		game::SceneNode getPlayer(void);
		void beHAVE(void);
		void setForward(glm::vec3 newFwd);

	private:
		glm::vec3 origin;
		SceneNode * player;
};      

#endif