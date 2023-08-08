#pragma once

#include <Engine/IScript.h>

namespace mh
{
	class Script_TestCH :
		public IScript
	{
	public:
		Script_TestCH();
		virtual ~Script_TestCH();


	private:
		int i;

	};

}

