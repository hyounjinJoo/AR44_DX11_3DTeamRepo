#include "PCH_Engine.h"

#include "guiComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

namespace gui
{
	guiComponent::guiComponent(mh::define::eComponentType _type)
		: guiChild(mh::define::strKey::ArrComName[(int)_type])
		, mType(_type)
		, mTarget(nullptr)
	{
	}

	guiComponent::~guiComponent()
	{

	}

}
