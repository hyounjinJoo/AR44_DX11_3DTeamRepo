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

	enum eKeyState
	{
		DOWN,
		PRESSED,
		UP,
		NONE,
	};



	class InputMgr
	{
		friend class Application;
	public:
		struct tKey
		{
			eKeyCode  eType;
			eKeyState eState;
			bool	  bPressed;
		};

		static __forceinline eKeyState GetKeyState(eKeyCode keyCode) { return mKeys[static_cast<uint>(keyCode)].eState; }
		static __forceinline float2 GetMousePos() { return mMousePos; }
		static __forceinline float2 GetMouseDir() { return mMouseDir; }

		//GetKey()		키를 누르는 시간만큼 true를 반환
		//GetKeyDown()	키를 눌렀을 때, 딱 한번 true를 반환
		//GetKeyUp()	키를 누르다 땠을 때, 딱 한번 true를 반환

		static __forceinline bool GetKey(eKeyCode keyCode)
		{
			return mKeys[static_cast<uint>(keyCode)].eState == eKeyState::PRESSED;
		}

		static __forceinline bool GetKeyDown(eKeyCode keyCode)
		{
			return mKeys[static_cast<uint>(keyCode)].eState == eKeyState::DOWN;
		}

		static __forceinline bool GetKeyUp(eKeyCode keyCode)
		{
			return mKeys[static_cast<uint>(keyCode)].eState == eKeyState::UP;
		}	

	private:
		static void Init();
		static void Update();
		static void Release();

	private:
		static std::vector<tKey> mKeys;
		static float2 mMousePos;
		static float2 mMousePosPrev;
		static float2 mMouseDir;
	};
}
