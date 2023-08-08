#include "PCH_Engine.h"
#include "Application.h"

#include "AtExit.h"

#include "DefaultComInitializer.h"
#include "RenderMgr.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResMgr.h"
#include "CollisionMgr.h"
#include "AudioMgr.h"
#include "FontWrapper.h"
#include "PathMgr.h"

namespace mh
{
	using namespace mh::define;

	std::vector<std::function<void()>> Application::mDestroyFuncs;


	HWND			Application::mHwnd{};
	HDC				Application::mHdc{};
	bool			Application::mbInitialized{};

	BOOL Application::Init(const tDesc_Application& _AppDesc)
	{
		AtExit::AddFunc(Release);

		if (nullptr == _AppDesc.Hwnd)
		{
			return FALSE;
		}
		mHwnd = _AppDesc.Hwnd;

		SetWindowPos(_AppDesc.LeftWindowPos, _AppDesc.TopWindowPos);
		SetWindowSize(_AppDesc.Width, _AppDesc.Height);

		if (false == GPUMgr::Init(_AppDesc.GPUDesc))
		{
			mHdc = GetDC(_AppDesc.Hwnd);
			ERROR_MESSAGE_W(L"Graphics Device 초기화 실패");
			return FALSE;
		}


		PathMgr::Init();

		AudioMgr::Init();
		FontWrapper::Init();

		ResMgr::Init();

		RenderMgr::Init();

		DefaultComInitializer::Init();
		
		TimeMgr::Init();
		InputMgr::Init();

		CollisionMgr::Init();
		
		SceneMgr::Init();

		mbInitialized = true;

		return TRUE;
	}

	// 게임 로직 캐릭터 이동 등등 
	// CPU UPDATE
	void Application::Update()
	{
		TimeMgr::Update();
		InputMgr::Update();
		CollisionMgr::Update();
		SceneMgr::Update();
	}

	// GPU update
	void Application::FixedUpdate()
	{
		CollisionMgr::FixedUpdate();
		SceneMgr::FixedUpdate();
	}

	void Application::Render()
	{
		TimeMgr::Render(mHdc);

		GPUMgr::Clear();
	
		RenderMgr::ClearMultiRenderTargets();

		RenderMgr::Render();
	}

	void Application::Destroy()
	{
		for (size_t i = 0; i < mDestroyFuncs.size(); ++i)
		{
			if (mDestroyFuncs[i])
			{
				mDestroyFuncs[i]();
			}
		}

		mDestroyFuncs.clear();
	}

	// Running main engine loop
	bool Application::Run()
	{
		Update();
		FixedUpdate();
		Render();
		Destroy();
		
		//TODO: Engine 내부에서 종료할 방법 만들기
		//이걸 false로 반환하면 꺼지도록 짜놓음
		return mbInitialized;
	}

	void Application::Present()
	{
		GPUMgr::Present();
	}

	void Application::Release()
	{
		Destroy();
		ReleaseDC(mHwnd, mHdc);
		mbInitialized = false;
	}

	void Application::SetWindowPos(int _LeftWindowPos, int _TopWindowPos)
	{
		//가로세로 길이는 유지하고 위치만 변경
		UINT flag = SWP_NOSIZE | SWP_NOZORDER;
		::SetWindowPos(mHwnd, nullptr, _LeftWindowPos, _TopWindowPos, 0, 0, flag);
	}
	void Application::SetWindowSize(int _width, int _height)
	{
		//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
		RECT rcWindow, rcClient;
		GetWindowRect(mHwnd, &rcWindow);
		GetClientRect(mHwnd, &rcClient);

		// calculate size of non-client area
		int xExtra = rcWindow.right - rcWindow.left - rcClient.right;
		int yExtra = rcWindow.bottom - rcWindow.top - rcClient.bottom;

		// now resize based on desired client size
		::SetWindowPos(mHwnd, 0, 0u, 0u, _width + xExtra, _height + yExtra, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		::ShowWindow(mHwnd, true);
		::UpdateWindow(mHwnd);
	}

	int2 Application::GetWIndowSize()
	{
		//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
		RECT rcClient{};
		GetClientRect(mHwnd, &rcClient);

		return int2{ rcClient.right, rcClient.bottom };
	}

}
