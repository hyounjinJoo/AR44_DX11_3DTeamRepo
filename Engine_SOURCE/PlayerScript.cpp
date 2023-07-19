#include "PlayerScript.h"
#include "Transform.h"
#include "GameObject.h"
#include "Input.h"
#include "Time.h"
#include "Animator.h"

namespace mh
{
	PlayerScript::PlayerScript()
		: Script()
	{
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
	}

	void PlayerScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		if (Input::GetKey(eKeyCode::RIGHT))
		{
			Vector3 pos = tr->GetPosition();
			pos.x += 60.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}
		if (Input::GetKey(eKeyCode::LEFT))
		{
			Vector3 pos = tr->GetPosition();
			pos.x -= 60.0f * Time::DeltaTime();
			tr->SetPosition(pos);
		}

		if (Input::GetKey(eKeyCode::DOWN))
		{
			Vector3 pos = tr->GetRotation();
			pos.y -= 60.0f * Time::DeltaTime();
			tr->SetRotation(pos);
		}
		if (Input::GetKey(eKeyCode::UP))
		{
			Vector3 pos = tr->GetRotation();
			pos.y += 60.0f * Time::DeltaTime();
			tr->SetRotation(pos);
		}

		Animator* animator = GetOwner()->GetComponent<Animator>();
		if (Input::GetKey(eKeyCode::N_1))
		{
			animator->Play(L"MoveDown");
		}
	}

	void PlayerScript::Render()
	{
	}

	void PlayerScript::OnCollisionEnter(Collider2D* _collider)
	{
	}

	void PlayerScript::OnCollisionStay(Collider2D* _collider)
	{
	}

	void PlayerScript::OnCollisionExit(Collider2D* _collider)
	{
	}

	void PlayerScript::Start()
	{
	}

	void PlayerScript::Action()
	{
	}

	void PlayerScript::End()
	{
	}

}
