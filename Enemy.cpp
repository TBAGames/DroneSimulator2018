#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string name, const game::Resource *geometry, const game::Resource *material, const game::Resource *texture, glm::vec3 originPoint, game::SceneNode *play, std::string t) : game::SceneNode(name, geometry, material, texture)
{
	origin = originPoint;
	player = play;
	SetPosition(origin);
	type = t;
	jump = false;
}

glm::vec3 Enemy::getOrigin(void)
{
	return origin;
}

game::SceneNode Enemy::getPlayer(void)
{
	return *player;
}

void Enemy::setForward(glm::vec3 newFwd)
{
	forward_ = newFwd;
}


void Enemy::beHAVE(void) //Required to be said like Austin Powers
{
	float distance = abs(glm::distance(GetPosition(), player->GetPosition()));
	glm::vec3 forwardTowardsPlayer = -glm::normalize(GetPosition() - player->GetPosition())/10.0f;
	//glm::vec3 forwardTowardsOrigin = glm::normalize(GetPosition() - getOrigin());
	std::cout << distance << std::endl;
	if (type == "Bird") {
		if (distance < 50.0)
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

	else if (type == "Dog") 
	{
		if (jump == true && GetPosition().y > -50.0f) 
		{
			Translate(glm::vec3(0.0f, -1.5f, 0.0f));
		}
		else if (jump == true && GetPosition().y <= -50.0f) 
		{
			jump = false;
		}
		else if (jump == false && GetPosition().y < 0.0f) 
		{
			Translate(glm::vec3(0.0f, 1.5f, 0.0f));
		}
		else if (distance < 100.0f && jump == false)
		{
			setForward(forwardTowardsPlayer);
			Translate(glm::vec3(forwardTowardsPlayer.x, 0.0f, forwardTowardsPlayer.z));
			if (abs(glm::distance(glm::vec3(GetPosition().x, 0.0f, GetPosition().z), glm::vec3(player->GetPosition().x, 0.0, player->GetPosition().z))) < 5.0)
			{
				jump = true;
			}
		}
		else
		{
			if (GetPosition() != getOrigin())
			{
				SetPosition(getOrigin());
			}
		}
		if (jump == false && GetPosition().y > -1.5f && GetPosition().y < 1.5f)
		{
			SetPosition(glm::vec3(GetPosition().x, 0.0, GetPosition().z));
		}
	}
}
