#include "EnginePCH.h"

#include "Script_Player.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "Input.h"
#include "TimeManager.h"
#include "Com_Animator.h"

namespace mh
{
	Script_Player::Script_Player()
		: IScript()
	{
	}

	Script_Player::~Script_Player()
	{
	}

	void Script_Player::Initialize()
	{
	}

	void Script_Player::Update()
	{
		Com_Transform& tr = GetOwner()->GetTransform();

		if (Input::GetKey(eKeyCode::RIGHT))
		{
			Vector3 pos = tr.GetPosition();
			pos.x += 60.0f * TimeManager::DeltaTime();
			tr.SetPosition(pos);
		}
		if (Input::GetKey(eKeyCode::LEFT))
		{
			Vector3 pos = tr.GetPosition();
			pos.x -= 60.0f * TimeManager::DeltaTime();
			tr.SetPosition(pos);
		}

		if (Input::GetKey(eKeyCode::DOWN))
		{
			Vector3 pos = tr.GetRotation();
			pos.y -= 60.0f * TimeManager::DeltaTime();
			tr.SetRotation(pos);
		}
		if (Input::GetKey(eKeyCode::UP))
		{
			Vector3 pos = tr.GetRotation();
			pos.y += 60.0f * TimeManager::DeltaTime();
			tr.SetRotation(pos);
		}

		Com_Animator* animator = GetOwner()->GetComponent<Com_Animator>();
		if (Input::GetKey(eKeyCode::N_1))
		{
			animator->Play("MoveDown");
		}
	}

	void Script_Player::Render()
	{
	}

	void Script_Player::OnCollisionEnter(ICollider2D* _collider)
	{
	}

	void Script_Player::OnCollisionStay(ICollider2D* _collider)
	{
	}

	void Script_Player::OnCollisionExit(ICollider2D* _collider)
	{
	}

	void Script_Player::Start()
	{
	}

	void Script_Player::Action()
	{
	}

	void Script_Player::End()
	{
	}

}
