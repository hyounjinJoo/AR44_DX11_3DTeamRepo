#pragma once

#include <Engine/IScript.h>

namespace mh
{
	class Script_TestScript
		: public IScript
	{
	public:
		Script_TestScript();

		Script_TestScript(const Script_TestScript& _other) = default;
		CLONE(Script_TestScript);

		virtual ~Script_TestScript();

		virtual void Init();
		virtual void Update();
	};
}



