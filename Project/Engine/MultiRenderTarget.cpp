#include "EnginePCH.h"
#include "MultiRenderTarget.h"

#include "define_Macro.h"
#include "Texture.h"

namespace mh
{
	MultiRenderTarget::MultiRenderTarget()
		: mMRTTextures{}
		, mMRTViews{}
		, mDSTexture{}
		, mRTCount{}
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

		//렌더타겟의 갯수를 카운트(0번부터 채워야 하고, 실제 렌더링 시에는 역순으로 렌더링)
		for (int i = 0; i < MRT_MAX; ++i)
		{
			if (_texture[i].get() == nullptr)
			{
				mRTCount = i;
				break;
			}

			mMRTTextures[i] = _texture[i];
			mMRTViews[i] = _texture[i]->GetRTV().Get();
		}

		mDSTexture = _dsTexture;

		return true;
	}


	void MultiRenderTarget::SetMultiRenderTargets()
	{
		GPUMgr::Context()->OMSetRenderTargets(mRTCount, mMRTViews, mDSTexture->GetDSV().Get());
	}
}
