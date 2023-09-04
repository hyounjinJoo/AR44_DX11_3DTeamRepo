#include "PCH_Engine.h"
#include "Script_Player.h"

#include "InputMgr.h"
#include "TimeMgr.h"

#include "GameObject.h"
#include "Com_Transform.h"
#include "Com_RigidBody.h"
#include "Com_Camera.h"
#include "RenderMgr.h"

namespace mh
{
	Script_Player::Script_Player()
		: IScript()
		, mMoveSpeed(1.0f)
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

	void Script_Player::FixedUpdate()
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
