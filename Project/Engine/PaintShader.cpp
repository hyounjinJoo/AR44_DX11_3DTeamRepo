#include "EnginePCH.h"

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

		mGroupX = static_cast<UINT>(mTarget->GetWidth() / mThreadGroupCountX + 1);
		mGroupY = static_cast<UINT>(mTarget->GetHeight() / mThreadGroupCountY + 1);
		mGroupZ = 1;
	}

	void PaintShader::Clear()
	{
		mTarget->UnBind();
	}
}
