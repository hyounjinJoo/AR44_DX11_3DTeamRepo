#include "PCH_Engine.h"
#include "MultiRenderTarget.h"

#include "define_Macro.h"
#include "Texture.h"

namespace mh
{
	MultiRenderTarget::MultiRenderTarget()
		: mRenderTargetTextures{}
		, mRenderTargetViews{}
		, mDSTexture{}
		, mRenderTargetCount{}
	{
	}
	MultiRenderTarget::~MultiRenderTarget()
	{
	}
	bool MultiRenderTarget::Create(std::shared_ptr<Texture> _texture[MRT_MAX], std::shared_ptr<Texture> _dsTexture)
	{
		if (nullptr == _texture[0])
		{
			ERROR_MESSAGE_W(L"렌더타겟이 하나도 없습니다.");
			MH_ASSERT(false);
			return false;
		}

		//렌더타겟의 갯수를 카운트
		for (int i = 0; i < MRT_MAX; ++i)
		{
			if (_texture[i].get() == nullptr)
			{
				mRenderTargetCount = i;
				break;
			}

			mRenderTargetTextures[i] = _texture[i];
			mRenderTargetViews[i] = _texture[i]->GetRTV().Get();
		}

		mDSTexture = _dsTexture;

		return true;
	}


	void MultiRenderTarget::Bind()
	{

		for (uint i = 0; i < mRenderTargetCount; ++i)
		{
			mRenderTargetTextures[i]->UnBindData();
		}

		ID3D11DepthStencilView* pDSView = nullptr;
		if (mDSTexture)
		{
			pDSView = mDSTexture->GetDSV().Get();
		}

		GPUMgr::Context()->OMSetRenderTargets(mRenderTargetCount, mRenderTargetViews, pDSView);
	}




	void MultiRenderTarget::Clear(const float4& _clearColor)
	{
		for (uint i = 0u; i < mRenderTargetCount; ++i)
		{
			if (mRenderTargetViews[i])
			{
				GPUMgr::Context()->ClearRenderTargetView(mRenderTargetViews[i], (FLOAT*)&_clearColor);
			}
		}

		if (mDSTexture)
		{
			constexpr uint ClearFlag = D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL;
			GPUMgr::Context()->ClearDepthStencilView(mDSTexture->GetDSV().Get(), ClearFlag, 1.f, (UINT8)0);
		}
	}

}
