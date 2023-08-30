#pragma once
#include "ComputeShader.h"
#include "Texture.h"

namespace mh
{
	class PaintShader : public ComputeShader
	{
	public:
		PaintShader();
		~PaintShader();

		virtual bool BindData() override;
		virtual void UnBindData() override;
		
		void SetTarget(std::shared_ptr<Texture> _texture) { mTarget = _texture; }

	private:
		std::shared_ptr<Texture> mTarget;
	};
}
