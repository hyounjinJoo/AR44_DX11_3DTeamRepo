#include "EnginePCH.h"
#include "Application.h"

#include "DefaultComInitializer.h"
#include "RenderMgr.h"
#include "TimeMgr.h"
#include "Input.h"
#include "SceneManager.h"
#include "ResMgr.h"
#include "CollisionMgr.h"
#include "Fmod.h"
#include "FontWrapper.h"

#include "PathMgr.h"

namespace mh
{
	Application::Application()
		: mHwnd(nullptr)
		, mHdc(nullptr)
		, mHeight(1600)//화면 해상도 몰라서 0말고 1600/900사이즈로 일단 초기화
		, mWidth(900)
		, mGraphicDevice()
	{
	}

	Application::~Application()
	{
	}

	void Application::Initialize()
	{
		DefaultComInitializer::Init();
		PathMgr::GetInst()->Init();

		ResMgr::GetInst();
		RenderMgr::GetInst()->Initialize();
		
		TimeMgr::Initialize();
		Input::Initialize();
		Fmod::Initialize();
		FontWrapper::Initialize();

		CollisionMgr::Initialize();
		
		SceneManager::Initialize();
	}

	// 게임 로직 캐릭터 이동 등등 
	// CPU UPDATE
	void Application::Update()
	{
		TimeMgr::Update();
		Input::Update();
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

		mGraphicDevice->Clear();
		mGraphicDevice->AdjustViewPorts(mHwnd);

		RenderMgr::GetInst()->Render();
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
		mGraphicDevice->Present();
	}

	void Application::Release()
	{
		Fmod::Release();
		FontWrapper::Release();
	}

	void Application::SetWindow(HWND _hwnd, UINT _width, UINT _height)
	{
		if (mGraphicDevice == nullptr)
		{
			mHwnd = _hwnd;
			mHdc = GetDC(mHwnd);
			mWidth = _width;
			mHeight = _height;


			//eValidationMode vaildationMode = eValidationMode::Disabled;
			
			mGraphicDevice = GPUMgr::GetInst();
			if (false == mGraphicDevice->Initialize(mHwnd, mWidth, mHeight))
			{
				ERROR_MESSAGE_W(L"Graphics Device 초기화에 실패했습니다.");
				std::abort();
			}
			//GPUMgr::GetInst() = mGraphicDevice.get();
		}

		RECT rt = { 0, 0, (LONG)_width , (LONG)_height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

}
