#pragma once
#include "IRes.h"


namespace mh
{
	class IShader :
		public IRes
	{
	public:
		IShader(define::eResourceType _ResType);
		virtual ~IShader();


	};
}


