#ifndef BIRD_H_
#define BIRD_H_

#include "scene_node.h"

class Enemy : public game::SceneNode 
{
	public:
		enum EnemyType { Bird, Dog, MailMan };
		Enemy(const std::string name, const game::Resource *geometry, const game::Resource *material, const game::Resource *texture, glm::vec3 originPoint, SceneNode *play, std::string t);
		glm::vec3 getOrigin(void);
		game::SceneNode getPlayer(void);
		void beHAVE(void);
		void setForward(glm::vec3 newFwd);

	private:
		glm::vec3 origin;
		SceneNode * player;
		std::string type;
		bool jump;
};

#endif