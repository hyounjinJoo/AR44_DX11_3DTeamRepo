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

		static void Clear(uint _startSlot);
		static void ClearAll();

		//생성 관련 함수
		bool Create(UINT _width, UINT _height, DXGI_FORMAT _format, uint _D3D11_BIND_FLAG, bool _bAllowCPURead = false);
		bool Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture);
		bool Create(const D3D11_TEXTURE2D_DESC& _TexDesc);

		//Save / Load
		virtual eResult Load(const std::filesystem::path& _path) override;
		void InitializeResource();


		void BindDataSRV(uint _SRVSlot, eShaderStageFlag_ _stageFlag);
		void BindDataUAV(uint _UAVSlot = 0u);
		void UnBind();


		void SetTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture) { mTexture = _texture; }

		uint GetHeight() const { return mDesc.Height; }
		uint GetWidth() const { return mDesc.Width; }

		ComPtr<ID3D11Texture2D> GetTexture() { return mTexture; }
		ComPtr<ID3D11DepthStencilView> GetDSV() { return mDSV; }
		ComPtr<ID3D11RenderTargetView> GetRTV() { return mRTV; }
		ComPtr<ID3D11UnorderedAccessView> GetUAV() { return mUAV; }
		ComPtr<ID3D11ShaderResourceView> GetSRV() { return mSRV; }

	private:
		eResult LoadFile(const std::filesystem::path& _fullPath);
		bool CreateView();

	private:
		D3D11_TEXTURE2D_DESC					mDesc;
		ComPtr<ID3D11Texture2D>					mTexture;
		ScratchImage							mImage;
		
		//각종 View
		//특정 버퍼와 연결하기 위한 통행증이라고 보면 됨
		ComPtr<ID3D11ShaderResourceView>		mSRV;
		ComPtr<ID3D11UnorderedAccessView>		mUAV;
		ComPtr<ID3D11DepthStencilView>			mDSV;
		ComPtr<ID3D11RenderTargetView>			mRTV;

		
		//현재 연결되어있는 버퍼 정보
		eBufferViewType							mCurBoundView;
		int										mCurBoundRegister;
		eShaderStageFlag_						mCurBoundStage;
	};

}
