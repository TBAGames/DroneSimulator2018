#ifndef DOG_H_
#define DOG_H_

#include "scene_node.h"

class dog : public game::SceneNode
{
public:
	dog(const std::string name, const game::Resource *geometry, const game::Resource *material, const game::Resource *texture, glm::vec3 originPoint, SceneNode *play);
	glm::vec3 getOrigin(void);
	game::SceneNode getPlayer(void);
	void beHAVE(void);
	void setForward(glm::vec3 newFwd);
	bool jump;

private:
	glm::vec3 origin;
	SceneNode * player;
};

#endif