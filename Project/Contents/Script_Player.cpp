#include "PCH_Contents.h"
#include "Script_Player.h"

#include <Engine/Animation3D.h>
#include <Engine/Com_Animator3D.h>
#include <Engine/GameObject.h>
#include <Engine/InputMgr.h>

namespace mh
{
	Script_Player::Script_Player()
		: mMoveSpeed(1.0f)
	{
	}

	Script_Player::~Script_Player()
	{
	}

	void Script_Player::Update()
	{
		if (InputMgr::GetKeyDown(eKeyCode::P))
		{
			Com_Animator3D* animator = GetOwner()->GetComponent<Com_Animator3D>();
			if (animator)
			{
				animator->PlayNext();
			}
		}

		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		Com_RigidBody* rb = GetOwner()->GetComponent<Com_RigidBody>();

		Com_Transform* camTr = RenderMgr::GetMainCam()->GetOwner()->GetComponent<Com_Transform>();
		float3 forward = camTr->GetForward();
		float3 right = camTr->GetRight();
		right.y = 0.f;


		if (InputMgr::GetKey(eKeyCode::H))
		{
			tr->SetPosition(float3(0.0f, 0.0f, 0.f));
		}

		if (InputMgr::GetKey(eKeyCode::W))
		{
			mMoveSpeed += 2.0f * TimeMgr::DeltaTime();
			rb->SetVelocity(define::eAxis3D::Z, mMoveSpeed);
		}

		if (InputMgr::GetKey(eKeyCode::S))
		{
			rb->SetVelocity(define::eAxis3D::Z, -mMoveSpeed);
		}

		if (InputMgr::GetKey(eKeyCode::A))
		{
			rb->AddForce((right * -100000.0f * 30.f));
			//rb->SetVelocity(define::eAxis3D::X, -mMoveSpeed);
		}

		if (InputMgr::GetKey(eKeyCode::D))
		{
			rb->SetVelocity(define::eAxis3D::X, mMoveSpeed);
		}

		if (InputMgr::GetKey(eKeyCode::SPACE))
		{
			rb->SetVelocity(define::eAxis3D::Y, mMoveSpeed);
		}
	}
}

