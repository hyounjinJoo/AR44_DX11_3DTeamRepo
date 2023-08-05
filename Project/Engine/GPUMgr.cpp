#include "PCH_Engine.h"
#include "GPUMgr.h"

#include "AtExit.h"

#include "Application.h"
#include "ConstBuffer.h"
#include "Mesh.h"
#include "RenderMgr.h"
#include "ResMgr.h"
#include "Texture.h"


namespace mh
{
	using namespace mh::define;

	UINT GPUMgr::mResolutionX{};
	UINT GPUMgr::mResolutionY{};
	UINT GPUMgr::mRefreshRate{};

	ComPtr<ID3D11Device>			GPUMgr::mDevice{};
	ComPtr<ID3D11DeviceContext>		GPUMgr::mContext{};

	ComPtr<IDXGISwapChain>			GPUMgr::mSwapChain{};

	std::shared_ptr<mh::Texture>	GPUMgr::mRenderTargetTexture{};
	std::shared_ptr<mh::Texture>	GPUMgr::mDepthStencilBufferTexture{};


	D3D11_VIEWPORT GPUMgr::mViewPort;

	bool GPUMgr::Init(const tDesc_GPUMgr& _Desc)
	{
		AtExit::AddFunc(GPUMgr::Release);

		/// 1. Device 와 SwapChain 생성한다.
		/// 2. 백버퍼에 실제로 렌더링할 렌더타겟 뷰를 생성해야한다.
		/// 3. 화면을 클리어 해줘야한다. 뷰포트를 생성해줘야 한다.
		/// 4. 매프레임마다 위에서 생성한 렌더타겟뷰에 렌더링해주어야한다.
		/// 5. Swapchain을 이용하여 최종 디바이스(디스플레이)에 화면을 그려줘야한다.
		if (false == CreateDevice(mDevice.GetAddressOf(), mContext.GetAddressOf()))
		{
			ERROR_MESSAGE_W(L"GPU 초기화 작업 실패");
			return false;
		}

		if (false == SetResoulution(_Desc.ResolutionX, _Desc.ResolutionY))
		{
			ERROR_MESSAGE_W(L"GPU 초기화 작업 실패");
			return false;
		}

		CreateMainViewPort();

		return true;
	}

	void GPUMgr::Release()
	{
		Reset();
	}

	void GPUMgr::Reset()
	{
		mViewPort = D3D11_VIEWPORT{};
		mDepthStencilBufferTexture = nullptr;
		mRenderTargetTexture = nullptr;
		mSwapChain = nullptr;
		mContext = nullptr;
		mDevice = nullptr;
	}

	bool GPUMgr::CreateDevice(ID3D11Device** _ppDevice, ID3D11DeviceContext** _ppContext)
	{
		bool Result = false;
		// Device, Device Context
		uint DeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;

		if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
			, DeviceFlag, nullptr, 0
			, D3D11_SDK_VERSION
			, _ppDevice
			, &FeatureLevel
			, _ppContext)))
		{
			_ppDevice = nullptr;
			_ppContext = nullptr;
			ERROR_MESSAGE_W(L"Graphics Device 생성에 실패했습니다.");
		}
		else
		{
			Result = true;
		}

		return Result;
	}

	bool GPUMgr::SetResoulution(UINT _ResolutionX, UINT _ResolutionY)
	{
		bool bResult = false;
		//1. 스왑체인 및 최종 렌더타겟 생성
		std::shared_ptr<Texture> RenderTex = CreateSwapChain(_ResolutionX, _ResolutionY, mRefreshRate);
		if (nullptr == RenderTex)
		{
			ERROR_MESSAGE_W(L"렌더타겟 생성 실패");
			return false;
		}
		mRenderTargetTexture = RenderTex;

		//3. DS 텍스처 다시 생성
		std::shared_ptr<Texture> DSTex = CreateDepthStencil(_ResolutionX, _ResolutionY);
		if (nullptr == DSTex)
		{
			ERROR_MESSAGE_W(L"Depth-Stencil 텍스처 생성 실패");
			return false;
		}
		mDepthStencilBufferTexture = DSTex;
		
		mResolutionX = _ResolutionX;
		mResolutionY = _ResolutionY;
		return true;
	}

	std::shared_ptr<Texture> GPUMgr::CreateSwapChain(UINT _ResolutionX, UINT _ResolutionY, UINT _RefreshRate)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.OutputWindow = Application::GetHwnd();
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.Width = _ResolutionX;
		swapChainDesc.BufferDesc.Height = _ResolutionY;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = _RefreshRate;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
		{
			return nullptr;
		}

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf())))
		{
			return nullptr;
		}

		if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf())))
		{
			return nullptr;
		}

		if (FAILED(pDXGIFactory->CreateSwapChain(mDevice.Get(), &swapChainDesc, mSwapChain.GetAddressOf())))
		{
			return nullptr;
		}

		
		//렌더타겟 생성
		std::shared_ptr<Texture> ReturnTex = std::make_shared<Texture>();
		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget = nullptr;

		// Get rendertarget for swapchain
		if (FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)renderTarget.GetAddressOf())))
		{
			ERROR_MESSAGE_W(L"스왑체인으로부터 렌더타겟을 받아오는 데 실패했습니다.");
			return nullptr;
		}

		//스왑체인으로 받아온 텍스처 버퍼를 가지고 렌더타겟 텍스처 뷰(RTV) 생성
		if (false == ReturnTex->Create(renderTarget))
		{
			ERROR_MESSAGE_W(L"렌더타겟 텍스처 생성에 실패했습니다.");
			return nullptr;
		}

		return ReturnTex;
	}


	std::shared_ptr<Texture> GPUMgr::CreateDepthStencil(UINT _Width, UINT _Height)
	{
		std::shared_ptr<Texture> DSTex = std::make_shared<mh::Texture>();
		
		if (false == DSTex->Create(_Width, _Height, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL))
		{
			ERROR_MESSAGE_W(L"Depth Stencil 버퍼 생성에 실패했습니다.");
			return nullptr;
		}

		return DSTex;
	}

	void GPUMgr::CreateMainViewPort()
	{
		//창 속에서의 상대적인 좌표이므로 0부터 시작하면 됨
		mViewPort.TopLeftX = (FLOAT)0.f;
		mViewPort.TopLeftY = (FLOAT)0.f;
		mViewPort.MinDepth = (FLOAT)0.f;
		mViewPort.MaxDepth = (FLOAT)1.f;
		mViewPort.Width = (FLOAT)Application::GetWidth();
		mViewPort.Height = (FLOAT)Application::GetHeight();

		mContext->RSSetViewports(1u, &mViewPort);
	}



	void GPUMgr::Clear()
	{
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetTexture->GetRTV().Get(), backgroundColor);
		mContext->ClearDepthStencilView(mDepthStencilBufferTexture->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}


