#pragma once
#include "IComponent.h"

namespace mh
{
	class ITransform :
		public IComponent
	{
	public:
		ITransform();
		virtual ~ITransform();

		virtual void BindData() = 0;

	protected:

	private:

	};
}


