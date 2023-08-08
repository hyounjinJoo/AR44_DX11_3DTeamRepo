
#include "PCH_Engine.h"

#include "InputMgr.h"
#include "Application.h"
#include "AtExit.h"



namespace mh
{
	

	std::vector<InputMgr::tKey> InputMgr::mKeys{};
	float2 InputMgr::mMousePos{};
	float2 InputMgr::mMousePosPrev{};
	float2 InputMgr::mMouseDir{};
	int ASCII[(uint)eKeyCode::END] =
	{
		//Alphabet
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		
		//Special Key
		VK_RETURN, VK_ESCAPE, VK_LSHIFT, VK_LMENU, VK_LCONTROL,
		VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_LBUTTON, VK_RBUTTON,

		//Num Pad
		VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2,
		VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
		VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8,
		VK_NUMPAD9,

		//Num Line
		'0', '1', '2', '3', '4', '5',
		'6', '7', '8', '9',
	};
	


	void InputMgr::Init()
	{
		AtExit::AddFunc(InputMgr::Release);

		for (uint i = 0; i < (uint)eKeyCode::END; i++)
		{
			tKey key;
			key.eType = (eKeyCode)i;
			key.eState = eKeyState::NONE;
			key.bPressed = false;

			mKeys.push_back(key);
		}
	}

	void InputMgr::Update()
	{
		if (GetFocus())
		{
			//KEY
			for (uint i = 0; i < (uint)eKeyCode::END; ++i)
			{
				// 해당키가 현재 눌려있다.
				if (GetAsyncKeyState(ASCII[i]) & 0x8000)
				{
					// 이전 프레임에도 눌려 있었다.
					if (mKeys[i].bPressed)
						mKeys[i].eState = eKeyState::PRESSED;
					else
						mKeys[i].eState = eKeyState::DOWN;

					mKeys[i].bPressed = true;
				}
				else // 해당키가 현재 안눌려있다.
				{
					// 이전 프레임에는 눌려 있었다.
					if (mKeys[i].bPressed)
						mKeys[i].eState = eKeyState::UP;
					else // 이전 프레임에도 안눌려 있었다.
						mKeys[i].eState = eKeyState::NONE;

					mKeys[i].bPressed = false;
				}
			}
			
			//Mouse 위치 갱신
			mMousePosPrev = mMousePos;

			POINT mousePos = {};
			GetCursorPos(&mousePos);
			ScreenToClient(Application::GetHwnd(), &mousePos);
			mMousePos.x = static_cast<float>(mousePos.x);
			mMousePos.y = static_cast<float>(mousePos.y);

			mMouseDir = mMousePos - mMousePosPrev;
			mMouseDir.y *= -1.f;
		}
		else
		{
			for (uint i = 0; i < (uint)eKeyCode::END; ++i)
			{
				if (eKeyState::DOWN == mKeys[i].eState || eKeyState::PRESSED == mKeys[i].eState)
					mKeys[i].eState = eKeyState::UP;
				else if (eKeyState::UP == mKeys[i].eState)
					mKeys[i].eState = eKeyState::NONE;

				mKeys[i].bPressed = false;
			}
		}
	}
	void InputMgr::Release()
	{
		mKeys.clear();
		mMousePos = {};
		mMouseDir = {};
	}
}
