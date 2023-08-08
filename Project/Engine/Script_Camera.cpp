#include "PCH_Engine.h"

#include "Script_Camera.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "InputMgr.h"
#include "TimeMgr.h"

namespace mh
{
	Script_Camera::Script_Camera()
		: IScript()
	{
	}

	Script_Camera::~Script_Camera()
	{
	}

	void Script_Camera::Init()
	{
	}

	void Script_Camera::Update()
	{
		Com_Transform& tr = GetOwner()->GetTransform();

		float3 pos = tr.GetPosition();

		if (InputMgr::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr.Right() * TimeMgr::DeltaTime();
		}
		else if (InputMgr::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr.Right() * TimeMgr::DeltaTime();
		}
		else if (InputMgr::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr.Forward() * TimeMgr::DeltaTime();
		}
		else if (InputMgr::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr.Forward() * TimeMgr::DeltaTime();
		}
		else if (InputMgr::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr.Up() * TimeMgr::DeltaTime();
		}
		else if (InputMgr::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr.Up() * TimeMgr::DeltaTime();
		}

		tr.SetPosition(pos);
	}
	void Script_Camera::Render()
	{
	}
}
