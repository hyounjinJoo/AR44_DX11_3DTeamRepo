#pragma once

#include "GameResource.h"
#include "GraphicDevice_DX11.h"


#include <DirectXTex/DirectXTex.h>


namespace mh::graphics
{
	using namespace mh::enums;
	class Texture : public GameResource
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
