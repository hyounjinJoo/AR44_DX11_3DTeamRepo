#pragma once
#include "yaComputeShader.h"
#include "yaTexture.h"

namespace ya::graphics
{
	class PaintShader : public ComputeShader
	{
	public:
		PaintShader();
		~PaintShader();

		virtual void Binds() override;
		virtual void Clear() override;
		
		void SetTarget(std::shared_ptr<Texture> texture) { mTarget = texture; }

	private:
		std::shared_ptr<Texture> mTarget;
	};
}
