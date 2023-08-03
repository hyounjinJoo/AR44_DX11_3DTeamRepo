#pragma once
#include "guiWidget.h"

namespace gui
{
	class YamYamEditor : public Widget
	{
	public:
		YamYamEditor();
		~YamYamEditor();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		bool mbPadding;
		bool mbFullscreen;
		ImGuiDockNodeFlags mDockspace_flags;
	};
}