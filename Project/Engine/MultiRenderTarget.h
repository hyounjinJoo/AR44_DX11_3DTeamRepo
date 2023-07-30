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
		void SetMultiRenderTargets();

	private:
		std::shared_ptr<Texture>	mMRTTextures[MRT_MAX];
		ID3D11RenderTargetView* mMRTViews[MRT_MAX];
		std::shared_ptr<Texture>	mDSTexture;
		UINT mRTCount;
    };
}


