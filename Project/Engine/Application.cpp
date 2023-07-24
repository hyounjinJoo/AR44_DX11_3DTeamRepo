#include "EnginePCH.h"

#include "Application.h"
#include "Renderer.h"
#include "TimeManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "GameResources.h"
#include "CollisionManager.h"
#include "Fmod.h"
#include "FontWrapper.h"

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

	void Application::Initialize()
	{
		TimeManager::Initialize();
		Input::Initialize();
		Fmod::Initialize();
		FontWrapper::Initialize();

		CollisionManager::Initialize();
		renderer::Initialize();
		SceneManager::Initialize();
	}

	// 게임 로직 캐릭터 이동 등등 
	// CPU UPDATE
	void Application::Update()
	{
		TimeManager::Update();
		Input::Update();
		CollisionManager::Update();
		SceneManager::Update();
	}

	// GPU update
	void Application::FixedUpdate()
	{
		CollisionManager::FixedUpdate();
		SceneManager::FixedUpdate();
	}

	void Application::Render()
	{
		TimeManager::Render(mHdc);

		graphicDevice->Clear();
		graphicDevice->AdjustViewPorts();

		renderer::Render();
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
		graphicDevice->Present();
	}

	void Application::Release()
	{
		GameResources::deleteTest();
		Fmod::Release();
		FontWrapper::Release();
	}

	void Application::SetWindow(HWND _hwnd, UINT _width, UINT _height)
	{
		if (graphicDevice == nullptr)
		{
			mHwnd = _hwnd;
			mHdc = GetDC(mHwnd);
			mWidth = _width;
			mHeight = _height;


			graphics::eValidationMode vaildationMode = graphics::eValidationMode::Disabled;
			graphicDevice = std::make_unique<graphics::GraphicDevice_DX11>();
			//graphics::GetDevice() = graphicDevice.get();
		}

		RECT rt = { 0, 0, (LONG)_width , (LONG)_height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

}
