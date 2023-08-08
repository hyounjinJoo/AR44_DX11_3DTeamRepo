#pragma once

#include <Engine/IScript.h>

namespace mh
{
	class Script_TestSB :
		public IScript
	{
	public:
		Script_TestSB();
		virtual ~Script_TestSB();


	private:
		int i;
		int a;

	};

}

