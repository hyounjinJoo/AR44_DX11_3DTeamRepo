#pragma once

#include "EnumFlags.h"
#include "SimpleMath.h"


namespace mh
{
	enum class eKeyCode
	{
		//Alphabet
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L, 
		Z, X, C, V, B, N, M,
		
		//Special Key
		ENTER, ESC, LSHIFT, LALT, LCTRL,
		SPACE, LEFT, RIGHT, UP, DOWN,
		LBTN, RBTN,
		
		//Num Pad
		NUM_0, NUM_1, NUM_2,
		NUM_3, NUM_4, NUM_5,
		NUM_6, NUM_7, NUM_8,
		NUM_9,

		//Num Line
		N_0, N_1, N_2, N_3, N_4, N_5,
		N_6, N_7, N_8, N_9,
		
		END,
	};

	enum class eGamepadKeyCode : unsigned int
	{
		None = 0,
		Menu = 0x1,
		View = 0x2,
		A = 0x4,
		B = 0x8,
		X = 0x10,
		Y = 0x20,
		DPadUp = 0x40,
		DPadDown = 0x80,
		DPadLeft = 0x100,
		DPadRight = 0x200,
		LeftShoulder = 0x400,
		RightShoulder = 0x800,
		LeftThumbstick = 0x1000,
		RightThumbstick = 0x2000,
	};

	enum eKeyState
	{
		DOWN,
		PRESSED,
		UP,
		NONE,
	};

	class Joystick;

	class InputMgr
	{
	public:
		struct tKey
		{
			eKeyCode  eType;
			eKeyState eState;
			bool	  bPressed;
		};

	public:
		static void Init();
		static void Update();
		static void Release();

		static __forceinline eKeyState GetKeyState(eKeyCode keyCode) 
		{ 
			return mKeys[static_cast<UINT>(keyCode)].eState; 
		}

		static __forceinline math::Vector2 GetMousPosition()
		{
			return mMousPosition;
		}

		//GetKey()		키를 누르는 시간만큼 true를 반환
		//GetKeyDown()	키를 눌렀을 때, 딱 한번 true를 반환
		//GetKeyUp()	키를 누르다 땠을 때, 딱 한번 true를 반환

		static __forceinline bool GetKey(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::PRESSED;
		}

		static __forceinline bool GetKeyDown(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::DOWN;
		}

		static __forceinline bool GetKeyUp(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::UP;
		}		

	private:
		static void InitializeGamePad();

	private:
		static std::vector<tKey> mKeys;
		static math::Vector2 mMousPosition;
		static std::vector<std::unique_ptr<Joystick>> mJoysticks;
	};
}
