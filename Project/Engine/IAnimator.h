#pragma once
#include "IComponent.h"


namespace mh
{
	class IAnimator :
		public IComponent
	{
	public:
		IAnimator(define::eDimensionType _type);
		virtual ~IAnimator() {};

		virtual void BindData() = 0;
		virtual void UnBindData() = 0;



		define::eDimensionType GetDimensionType() const { return mDimensionType; }

	private:
		define::eDimensionType mDimensionType;
	};
}


