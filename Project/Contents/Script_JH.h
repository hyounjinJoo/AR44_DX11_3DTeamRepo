#pragma once

#include <Engine/IScript.h>

namespace mh
{
	class Script_JH :
		public IScript
	{
	public:
		Script_JH();
		virtual ~Script_JH();


		virtual void Init() override;

	private:
		int TEST_JH;
	};
}


