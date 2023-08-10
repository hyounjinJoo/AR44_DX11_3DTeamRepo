#pragma once
#include "guiBase.h"

namespace gui
{
	class Game : public guiBase
	{
	public:
		Game();
		virtual ~Game();

		virtual void Update() override;

	private:
	};
}
