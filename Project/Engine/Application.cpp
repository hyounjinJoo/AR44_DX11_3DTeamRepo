#include "EnginePCH.h"
#include "Application.h"

#include "DefaultComInitializer.h"
#include "RenderMgr.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "SceneManager.h"
#include "ResMgr.h"
#include "CollisionMgr.h"
#include "AudioMgr.h"
#include "FontWrapper.h"

#include "PathMgr.h"

namespace mh
{
	Application::Application()
		: mHwnd(nullptr)
		, mHdc(nullptr)
		, mHeight(1600)//화면 해상도 몰라서 0말고 1600/900사이즈로 일단 초기화
		, mWidth(900)
	{
	}

	Application::~Application()
	{
	}

	void Application::Init()
	{
		DefaultComInitializer::Init();
		PathMgr::Init();
		ResMgr::Init();

		RenderMgr::Init();
		
		TimeMgr::Init();
		InputMgr::Init();
		AudioMgr::Init();
		FontWrapper::Init();

		CollisionMgr::Init();
		
		SceneManager::Init();
	}

	// 게임 로직 캐릭터 이동 등등 
	// CPU UPDATE
	void Application::Update()
	{
		TimeMgr::Update();
		InputMgr::Update();
		CollisionMgr::Update();
		SceneManager::Update();
	}

	// GPU update
	void Application::FixedUpdate()
	{
		CollisionMgr::FixedUpdate();
		SceneManager::FixedUpdate();
	}

	void Application::Render()
	{
		TimeMgr::Render(mHdc);

		GPUMgr::Clear();
		GPUMgr::AdjustViewPorts(mHwnd);

		RenderMgr::Render();
	}

	void Application::Destroy()
	{

	}

	// Running main engine loop
	void Application::Run()
	{
		Update();
		FixedUpdate();
		Render();
		Destroy();
	}

	void Application::Present()
	{
		GPUMgr::Present();
	}

	void Application::Release()
	{
		AtExit::CallAtExit();
	}

	void Application::SetWindow(HWND _hwnd, UINT _width, UINT _height)
	{
		if (nullptr == GPUMgr::Device())
		{
			mHwnd = _hwnd;
			mHdc = GetDC(mHwnd);
			mWidth = _width;
			mHeight = _height;


			//eValidationMode vaildationMode = eValidationMode::Disabled;
			
			if (false == GPUMgr::Init(mHwnd, mWidth, mHeight))
			{
				ERROR_MESSAGE_W(L"Graphics Device 초기화에 실패했습니다.");
				std::abort();
			}
		}

		RECT rt = { 0, 0, (LONG)_width , (LONG)_height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

}
