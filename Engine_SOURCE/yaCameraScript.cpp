#include "yaCameraScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"

namespace ya
{
	CameraScript::CameraScript()
		: Script()
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initalize()
	{
	}

	void CameraScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 pos = tr->GetPosition();

		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr->Right() * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr->Right() * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr->Foward() * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr->Foward() * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr->Up() * Time::DeltaTime();
		}
		else if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr->Up() * Time::DeltaTime();
		}

		tr->SetPosition(pos);
	}
	void CameraScript::Render()
	{
	}
}