
#include "EnginePCH.h"

#include "InputMgr.h"
#include "Application.h"
#include "Joystick.h"
#include "TimeMgr.h"


extern mh::Application gApplication;

namespace mh
{
	std::vector<InputMgr::tKey> InputMgr::mKeys{};
	math::Vector2 InputMgr::mMousPosition{};


	std::vector<std::unique_ptr<Joystick>> InputMgr::mJoysticks;
	int ASCII[(UINT)eKeyCode::END] =
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
		for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
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
			for (UINT i = 0; i < (UINT)eKeyCode::END; ++i)
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
			
			POINT mousePos = {};
			GetCursorPos(&mousePos);
			ScreenToClient(gApplication.GetHwnd(), &mousePos);
			mMousPosition.x = static_cast<float>(mousePos.x);
			mMousPosition.y = static_cast<float>(mousePos.y);

			InitGamePad();
		}
		else
		{
			for (UINT i = 0; i < (UINT)eKeyCode::END; ++i)
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
		mJoysticks.clear();
	}

	void InputMgr::InitGamePad()
	{
		static int callTimes = 0;
		callTimes++;

		static float elapsedTime = 0.f;
		elapsedTime += TimeMgr::DeltaTime();

		auto hr = RoInitialize(RO_INIT_MULTITHREADED);
		assert(SUCCEEDED(hr));
				
		Microsoft::WRL::ComPtr<ABI::Windows::Gaming::Input::IGamepadStatics> gamepadStatics;
		hr = RoGetActivationFactory(Microsoft::WRL::Wrappers::HStringReference(L"Windows.Gaming.Input.Gamepad").Get()
			, __uuidof(Microsoft::WRL::ComPtr<ABI::Windows::Gaming::Input::IGamepadStatics>), &gamepadStatics);
		assert(SUCCEEDED(hr));

		Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IVectorView<ABI::Windows::Gaming::Input::Gamepad*>> gamepads;
		hr = gamepadStatics->get_Gamepads(&gamepads);
		assert(SUCCEEDED(hr));

		uint32_t gamepadCount;
		hr = gamepads->get_Size(&gamepadCount);
		assert(SUCCEEDED(hr));

		for (uint32_t i = 0; i < gamepadCount; i++) {
			Microsoft::WRL::ComPtr<ABI::Windows::Gaming::Input::IGamepad> gamepad;
			hr = gamepads->GetAt(i, &gamepad);
			assert(SUCCEEDED(hr));

			bool isFind = false;

			size_t Size = mJoysticks.size();
			for (size_t index = 0; index < Size; ++index)
			{
				if (mJoysticks[index]->GetStick() == gamepad)
				{
					isFind = true;
					break;
				}
			}

			if (isFind)
			{
				continue;
			}
			// Joystick를 생성하고 gamepad를 전달.
			mJoysticks.push_back(std::make_unique<Joystick>(gamepad));
		}
	}
}
