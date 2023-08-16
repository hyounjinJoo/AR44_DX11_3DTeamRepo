#include "PCH_Engine.h"

#include "Script_Player.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Com_Animator2D.h"

namespace mh
{
	Script_Player::Script_Player()
		: IScript()
	{
	}

	Script_Player::~Script_Player()
	{
	}

	void Script_Player::Init()
	{
	}

	void Script_Player::Update()
	{
		Com_Transform& tr = GetOwner()->GetTransform();

		if (InputMgr::GetKey(eKeyCode::RIGHT))
		{
			float3 pos = tr.GetRelativePos();
			pos.x += 60.0f * TimeMgr::DeltaTime();
			tr.SetRelativePos(pos);
		}
		if (InputMgr::GetKey(eKeyCode::LEFT))
		{
			float3 pos = tr.GetRelativePos();
			pos.x -= 60.0f * TimeMgr::DeltaTime();
			tr.SetRelativePos(pos);
		}

		if (InputMgr::GetKey(eKeyCode::DOWN))
		{
			float rotY = tr.GetRelativeRotY();
			rotY -= 60.0f * TimeMgr::DeltaTime();
			tr.SetRelativeRotY(rotY);
		}
		if (InputMgr::GetKey(eKeyCode::UP))
		{
			float rotY = tr.GetRelativeRotY();
			rotY += 60.0f * TimeMgr::DeltaTime();
			tr.SetRelativeRotY(rotY);
		}

		Com_Animator2D* animator = GetOwner()->GetComponent<Com_Animator2D>();
		if (InputMgr::GetKey(eKeyCode::N_1))
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
