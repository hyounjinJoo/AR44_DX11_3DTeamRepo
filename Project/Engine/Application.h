#pragma once
#include "EnginePCH.h"
#include "define_GPU.h"
#include "GPUMgr.h"

namespace mh
{
	class Application
	{
	public:
		Application();
		~Application();

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		// Running main engine loop
		void Run();
		void Present();
		void Release();


		void SetWindow(HWND _hwnd, uint _width, uint _height);
		void SetHwnd(HWND _hwnd) { mHwnd = _hwnd; }
		HWND GetHwnd() const { return mHwnd; }
		uint GetWidth() const { return mWidth; }
		uint GetHeight() const { return mHeight; }

	private:
		bool bInitialized = false;

		HWND mHwnd;
		HDC mHdc;
		uint mHeight;
		uint mWidth;
	};
}
