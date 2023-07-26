#include "EnginePCH.h"

#include "Script_Camera.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "Input.h"
#include "TimeManager.h"

namespace mh
{
	Script_Camera::Script_Camera()
		: IScript()
	{
	}

	Script_Camera::~Script_Camera()
	{
	}

	void Script_Camera::Initialize()
	{
	}

	void Script_Camera::Update()
	{
		Com_Transform& tr = GetOwner()->GetTransform();

		Vector3 pos = tr.GetPosition();

		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr.Right() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr.Right() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr.Foward() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr.Foward() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr.Up() * TimeManager::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr.Up() * TimeManager::DeltaTime();
		}

		tr.SetPosition(pos);
	}
	void Script_Camera::Render()
	{
	}
}
