#pragma once
#include "GameResource.h"


namespace mh
{
	class Shader :
		public GameResource
	{
	public:
		Shader(enums::eResourceType _ResType);
		virtual ~Shader();


	};
}


