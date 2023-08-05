#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "define_Struct.h"

namespace mh
{


	class Application
	{
	public:
		//struct tDesc_Application
		//{
		//	HWND Hwnd;
		//	int  LeftWindowPos;
		//	int	 TopWindowPos;
		//	int  Width;
		//	int  Height;
		//	tDesc_GPUMgr GPUDesc;
		//};
		static BOOL Init(const define::tDesc_Application& _AppDesc);

		static void Update();
		static void FixedUpdate();
		static void Render();
		static void Destroy();

		// Running main engine loop
		static bool Run();
		static void Present();
		static void Release();

		static void SetWindowPos(int _LeftWindowPos, int _TopWindowPos);
		static void SetWindowSize(int _Width, int _Height);
		static HWND GetHwnd() { return mAppDesc.Hwnd; }
		static UINT GetWidth() { return mAppDesc.Width; }
		static UINT GetHeight() { return mAppDesc.Height; }

	private:
		static mh::define::tDesc_Application mAppDesc;
		static HDC  mHdc;
		static bool mbInitialized;

	public:
		Application() = delete;
		~Application() = delete;
	};
}
