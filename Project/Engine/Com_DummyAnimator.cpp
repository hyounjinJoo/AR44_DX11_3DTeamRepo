#include "PCH_Engine.h"
#include "Com_DummyAnimator.h"

#include "Com_Animator3D.h"
#include "GameObject.h"

namespace mh
{

	Com_DummyAnimator::Com_DummyAnimator()
		: IAnimator(eDimensionType::_3D)
		, mParentAnimator()
	{
	}

	Com_DummyAnimator::~Com_DummyAnimator()
	{
	}

	void Com_DummyAnimator::Init()
	{
		GameObject* parent = GetOwner()->GetParent();
		if (parent)
		{
			mParentAnimator = parent->GetComponent<Com_Animator3D>();
		}
	}

	void Com_DummyAnimator::BindData()
	{
		if (mParentAnimator)
			mParentAnimator->BindData();
	}

	void Com_DummyAnimator::UnBindData()
	{
		if (mParentAnimator)
			mParentAnimator->UnBindData();
	}
}

