#pragma once
#include "define_GPU.h"


#include "Singleton.h"
#include "define_Enum.h"


namespace mh
{
	using namespace mh::define;
	class Texture;

	class GPUMgr 
		: public Singleton<GPUMgr>
	{
		SINGLETON(GPUMgr);

	public:
		bool Initialize(HWND _hwnd, UINT _Width, UINT _Height);
		ComPtr<ID3D11Device> GetDevice() { return mDevice.Get(); }
		ComPtr<ID3D11DeviceContext> GetContext() { return mContext.Get(); }

		//================================ INLINE WRAPPER ====================================
	public:
		void SetData(ID3D11Buffer* _buffer, void* _data, UINT _size);
		void Clear() const;// 화면 지워주기
		void AdjustViewPorts(HWND _hWnd);
		void OMSetRenderTarget();
		void Present(bool _bVSync = false);

	private:
		void Reset();
		bool CreateDevice(ID3D11Device** _ppDevice, ID3D11DeviceContext** _ppContext);
		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* _desc);
		std::shared_ptr<Texture> CreateRenderTarget();
		std::shared_ptr<Texture> CreateDepthStencil(UINT _Width, UINT _Height);
		void CreateViewPort(HWND _hWnd);

	private:

		Microsoft::WRL::ComPtr <ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> mContext;
	
		Microsoft::WRL::ComPtr <IDXGISwapChain> mSwapChain;
		
		std::shared_ptr<mh::Texture> mRenderTargetTexture;
		std::shared_ptr<mh::Texture> mDepthStencilBufferTexture;

		D3D11_VIEWPORT mViewPort;
	};

	inline void GPUMgr::Present(bool _bVSync)
	{
		int VSync = true == _bVSync ? 1 : 0;
		mSwapChain->Present(VSync, 0u);
	}
}


