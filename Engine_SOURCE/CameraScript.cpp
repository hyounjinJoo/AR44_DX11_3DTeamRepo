#include "EnginePCH.h"

#include "CameraScript.h"
#include "Transform.h"
#include "GameObject.h"
#include "Input.h"
#include "TimeManager.h"

namespace mh
{
	CameraScript::CameraScript()
		: Script()
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initialize()
	{
	}

	void CameraScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 pos = tr->GetPosition();

		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr->Right() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr->Right() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr->Foward() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr->Foward() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr->Up() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr->Up() * TimeManager::DeltaTime();
		}

		tr->SetPosition(pos);
	}
	void CameraScript::Render()
	{
	}
}