#pragma once
#include "Entity.h"

#include "define_GPU.h"

namespace mh
{
	class Texture;
    class MultiRenderTarget :
        public Entity
    {
	public:
		MultiRenderTarget();
		virtual ~MultiRenderTarget();

		bool Create(std::shared_ptr<Texture> texture[MRT_MAX], std::shared_ptr<Texture> dsTexture);
		void Bind();
		inline std::shared_ptr<Texture> GetRenderTarget(UINT _uRenderTargetIndex);

		//렌더타겟을 특정 색상으로 변경
		void Clear(const Vector4& _clearColor);

	private:
		std::shared_ptr<Texture>	mRenderTargetTextures[MRT_MAX];
		ID3D11RenderTargetView*		mRenderTargetViews[MRT_MAX];
		std::shared_ptr<Texture>	mDSTexture;
		UINT mRenderTargetCount;
    };

	inline std::shared_ptr<Texture> MultiRenderTarget::GetRenderTarget(UINT _uRenderTargetIndex)
	{
		std::shared_ptr<Texture> RenderTarget = nullptr;
		if (_uRenderTargetIndex < MRT_MAX)
		{
			RenderTarget = mRenderTargetTextures[_uRenderTargetIndex];
		}

		return RenderTarget;
	}
}


