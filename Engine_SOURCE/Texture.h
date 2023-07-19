#pragma once
#include "..\External\DirectXTex\Include\DirectXTex.h"
#include "Resource.h"
#include "GraphicDevice_DX11.h"




using namespace mh::enums;
namespace mh::graphics
{
	class Texture : public Resource
	{
	public:
		Texture();
		virtual ~Texture();

		static void Clear(UINT _startSlot);

		bool Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag);
		bool Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture);
		virtual HRESULT Load(const std::wstring& _path) override;
		void LoadFile(const std::wstring& _name);
		void InitializeResource();
		void BindShaderResource(eShaderStage _stage, UINT _slot);
		void BindUnorderedAccessView(UINT _startSlot);
		void ClearUnorderedAccessView(UINT _startSlot);

		void Clear();

		void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture) { mTexture = _texture; }

		size_t GetHeight() { return mDesc.Height; }
		size_t GetWidth() { return mDesc.Width; }

		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture() { return mTexture; }
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDSV() { return mDSV; }
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRTV() { return mRTV; }
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> GetUAV() { return mUAV; }
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV() { return mSRV; }

	private:
		ScratchImage mImage;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDSV;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRTV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		D3D11_TEXTURE2D_DESC mDesc;
	};

}
