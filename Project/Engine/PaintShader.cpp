#include "PCH_Engine.h"

#include "PaintShader.h"

namespace mh
{
	PaintShader::PaintShader()
		: ComputeShader()
		, mTarget(nullptr)
	{
	}

	PaintShader::~PaintShader()
	{
	}

	void PaintShader::Binds()
	{
		mTarget->BindDataUAV(0);

		mGroupX = static_cast<uint>(mTarget->GetWidth() / mThreadGroupCountX + 1);
		mGroupY = static_cast<uint>(mTarget->GetHeight() / mThreadGroupCountY + 1);
		mGroupZ = 1;
	}

	void PaintShader::Clear()
	{
		mTarget->UnBind();
	}
}
