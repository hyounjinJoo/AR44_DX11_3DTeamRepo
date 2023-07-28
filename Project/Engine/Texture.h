#pragma once

#include "IRes.h"
#include "GPUMgr.h"

#include <DirectXTex/DirectXTex.h>


namespace mh
{
	using namespace mh::define;
	using Microsoft::WRL::ComPtr;

	class Texture : public IRes
	{
	public:
		Texture();
		virtual ~Texture();

		static void Clear(UINT _startSlot);

		bool Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag);
		bool Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture);
		virtual HRESULT Load(const std::filesystem::path& _path) override;
		HRESULT LoadFile(const std::filesystem::path& _fullPath);
		void InitializeResource();
		void BindShaderResource(eShaderStage _stage, UINT _slot);
		void BindUnorderedAccessView(UINT _startSlot);
		void ClearUnorderedAccessView(UINT _startSlot);

		void Clear();

		void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture) { mTexture = _texture; }

		size_t GetHeight() { return mDesc.Height; }
		size_t GetWidth() { return mDesc.Width; }

		ComPtr<ID3D11Texture2D> GetTexture() { return mTexture; }
		ComPtr<ID3D11DepthStencilView> GetDSV() { return mDSV; }
		ComPtr<ID3D11RenderTargetView> GetRTV() { return mRTV; }
		ComPtr<ID3D11UnorderedAccessView> GetUAV() { return mUAV; }
		ComPtr<ID3D11ShaderResourceView> GetSRV() { return mSRV; }

	private:
		ScratchImage mImage;
		ComPtr<ID3D11Texture2D> mTexture;
		ComPtr<ID3D11DepthStencilView> mDSV;
		ComPtr<ID3D11RenderTargetView> mRTV;
		ComPtr<ID3D11UnorderedAccessView> mUAV;
		ComPtr<ID3D11ShaderResourceView> mSRV;
		D3D11_TEXTURE2D_DESC mDesc;
	};

}
