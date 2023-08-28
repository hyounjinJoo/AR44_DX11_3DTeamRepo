#include "PCH_Engine.h"
#include "ITransform.h"

namespace mh
{
	ITransform::ITransform()
		: IComponent(define::eComponentType::Transform)
	{
	}
	ITransform::~ITransform()
	{
	}
}
