#include "PCH_Engine.h"
#include "PaintShader.h"


namespace mh
{
	PaintShader::PaintShader()
		: ComputeShader(uint3{32, 32, 1})
		, mTarget(nullptr)
	{
	}

	PaintShader::~PaintShader()
	{
	}

	bool PaintShader::BindData()
	{
		mTarget->BindDataUAV(0);

		uint3 dataCounts = { mTarget->GetWidth(), mTarget->GetHeight(), 1u };

		CalculateGroupCount(dataCounts);

		return true;
	}

	void PaintShader::UnBindData()
	{
		mTarget->UnBindData();
	}
}
