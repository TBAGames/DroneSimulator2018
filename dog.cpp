#include "dog.h"

dog::dog(const std::string name, const game::Resource *geometry, const game::Resource *material, const game::Resource *texture, glm::vec3 originPoint, game::SceneNode *play) : game::SceneNode(name, geometry, material, texture)
{
	origin = originPoint;
	player = play;
	SetPosition(origin);
	jump = false;
}

glm::vec3 dog::getOrigin(void)
{
	return origin;
}

game::SceneNode dog::getPlayer(void)
{
	return *player;
}

void dog::setForward(glm::vec3 newFwd)
{
	forward_ = newFwd;
}

void dog::beHAVE(void) //Required to be said like Austin Powers
{
	//float distance = glm::distance(GetPosition(), player->GetPosition());
	//glm::vec3 forwardTowardsPlayer = -glm::normalize(GetPosition() - player->GetPosition()) / 10.0f;
	//glm::vec3 forwardTowardsOrigin = glm::normalize(GetPosition() - getOrigin());

	//if (jump == true && GetPosition().y < 100.0f) 
	//{
	//	Translate(glm::vec3(0.0f, 1.5f, 0.0f));
	//}
	//else if (jump == true && GetPosition().y >= 100.0f) 
	//{
	//	jump = false;
	//}
	//else if (jump == false && GetPosition().y >= 0.0f) 
	//{
	//	Translate(glm::vec3(0.0f, -1.5f, 0.0f));
	//}
	//else if (jump == false && GetPosition().y <= 0.0f) 
	//{
	//	SetPosition(glm::vec3(GetPosition().x, 0.0, GetPosition().z));
	//}
	//else if (distance < 100.0f && jump == false)
	//{
	//	setForward(forwardTowardsPlayer);
	//	Translate(glm::vec3(forwardTowardsPlayer.x, 0.0f, forwardTowardsPlayer.z));
	//	if (distance < 5.0) 
	//	{
	//		jump = true;
	//	}
	//}
	//else
	//{
	//	if (GetPosition() != getOrigin())
	//	{
	//		SetPosition(getOrigin());
	//	}
	//}
	float distance = glm::distance(GetPosition(), player->GetPosition());
	glm::vec3 forwardTowardsPlayer = -glm::normalize(GetPosition() - player->GetPosition()) / 10.0f;
	glm::vec3 forwardTowardsOrigin = glm::normalize(GetPosition() - getOrigin());

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
