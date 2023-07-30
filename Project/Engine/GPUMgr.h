#pragma once
#include "define_GPU.h"

#include "define_Enum.h"


namespace mh
{
	class Texture;
	class GPUMgr 
	{
		friend class Application;
	public:
		__forceinline static ComPtr<ID3D11Device>			Device() { return mDevice; }
		__forceinline static ComPtr<ID3D11DeviceContext>	Context() { return mContext; }

	public:
		static void Clear();// 화면 지워주기
		static void AdjustViewPorts(HWND _hWnd);
		static inline void Present(bool _bVSync = false);

		static inline std::shared_ptr<mh::Texture> GetRenderTargetTex();
		static inline std::shared_ptr<mh::Texture> GetDepthStencilBufferTex();

	private:
		static bool Init(HWND _hwnd, UINT _Width, UINT _Height);
		static void Release();
		static void Reset();
		static bool CreateDevice(ID3D11Device** _ppDevice, ID3D11DeviceContext** _ppContext);
		static bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* _desc);
		static std::shared_ptr<Texture> CreateRenderTarget();
		static std::shared_ptr<Texture> CreateDepthStencil(UINT _Width, UINT _Height);

		

		static void CreateViewPort(HWND _hWnd);

	private:
		static ComPtr<ID3D11Device> mDevice;
		static ComPtr<ID3D11DeviceContext> mContext;
	
		static ComPtr<IDXGISwapChain> mSwapChain;
		
		static std::shared_ptr<mh::Texture> mRenderTargetTexture;
		static std::shared_ptr<mh::Texture> mDepthStencilBufferTexture;

		
		static D3D11_VIEWPORT mViewPort;
	};

	inline void GPUMgr::Present(bool _bVSync)
	{
		int VSync = true == _bVSync ? 1 : 0;
		mSwapChain->Present(VSync, 0u);
	}

	inline std::shared_ptr<mh::Texture> GPUMgr::GetRenderTargetTex()
	{
		return mRenderTargetTexture;
	}

	inline std::shared_ptr<mh::Texture> GPUMgr::GetDepthStencilBufferTex()
	{
		return mDepthStencilBufferTexture;
	}
}


