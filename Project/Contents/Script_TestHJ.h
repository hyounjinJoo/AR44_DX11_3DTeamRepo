#pragma once

#include <Engine/IScript.h>

namespace mh
{
	class Script_TestHJ
		: public IScript
	{
	public:
		Script_TestHJ();
		virtual ~Script_TestHJ();

		virtual void Init() override;
		virtual void Update() override;

		int GetHP() const;

	private:
		int HP_HJ;
	};

}

