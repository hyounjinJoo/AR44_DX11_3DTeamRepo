#include "PCH_Engine.h"
#include "ILight.h"

namespace mh
{
	ILight::ILight(define::eDimensionType _dimensionType)
		: IComponent(define::eComponentType::Light)
		, mDimension(_dimensionType)
	{
	}

	ILight::~ILight()
	{
	}
}


