#include "bird.h"


bird::bird(const std::string name, const game::Resource *geometry, const game::Resource *material, const game::Resource *texture, glm::vec3 originPoint, game::SceneNode *play) : game::SceneNode(name, geometry, material, texture)
{
	origin = originPoint;
	player = play;
	SetPosition(origin);
}

glm::vec3 bird::getOrigin(void)
{
	return origin;
}

game::SceneNode bird::getPlayer(void) 
{
	return *player;
}

void bird::setForward(glm::vec3 newFwd) 
{
	forward_ = newFwd;
}


void bird::beHAVE(void) //Required to be said like Austin Powers
{
	float distance = glm::distance(GetPosition(), player->GetPosition());
	glm::vec3 forwardTowardsPlayer = -glm::normalize(GetPosition() - player->GetPosition())/10.0f;
	glm::vec3 forwardTowardsOrigin = glm::normalize(GetPosition() - getOrigin());

	if (distance < 30.0) 
	{
		setForward(forwardTowardsPlayer);
		Translate(forwardTowardsPlayer);
	}
	else 
	{
		if (GetPosition() != getOrigin()) 
		{
			SetPosition(getOrigin());
		}
	}
}
