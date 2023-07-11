#pragma once
#include "guiWidget.h"


namespace gui
{
	class Game : public Widget
	{
	public:
		Game();
		~Game();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;


	private:
	};
}
