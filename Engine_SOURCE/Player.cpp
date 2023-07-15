#include "Player.h"


namespace mh
{
	Player::Player()
	{
	}

	Player::~Player()
	{
	}

	void Player::Initalize()
	{
		GameObject::Initalize();
	}

	void Player::Update()
	{
		GameObject::Update();
		//head->update();
		//body->update();
	}

	void Player::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Player::Render()
	{
		GameObject::Render();
	}
}
