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

		virtual void BindGPU() = 0;
		virtual void Clear() = 0;



		define::eDimensionType GetDimensionType() const { return mDimensionType; }

	private:
		define::eDimensionType mDimensionType;
	};
}


