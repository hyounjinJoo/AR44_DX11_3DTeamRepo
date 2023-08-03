#pragma once
#include "guiWidget.h"


namespace gui
{
	class Console : public Widget
	{
	public:
		Console();
		virtual ~Console();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;


	private:
	};
}
