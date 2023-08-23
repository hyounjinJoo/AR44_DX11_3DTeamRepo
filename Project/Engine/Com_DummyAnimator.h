#pragma once
#include "IAnimator.h"

namespace mh
{
	class Com_Animator3D;
	class Com_DummyAnimator :
		public IAnimator
	{
	public:
		Com_DummyAnimator();
		virtual ~Com_DummyAnimator();
		
		virtual void Init() override;
		virtual void FixedUpdate() override {};
		virtual void Render() override;

		virtual void BindData() override {}
		virtual void UnBindData() override {}

	private:
		Com_Animator3D* mParentAnimator;
	};
}


