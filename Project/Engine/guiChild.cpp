#include "PCH_Engine.h"
#include "guiChild.h"

gui::guiChild::guiChild(const std::string_view _strName)
    : guiWindow(_strName)
    , mbBorder(true)
	, mSize{}
{
}

gui::guiChild::~guiChild()
{
}

bool gui::guiChild::BeginUI()
{
	bool retVal = ImGui::BeginChild(GetKey().c_str(), mSize, mbBorder, GetImGuiWindowFlag());
	if (false == retVal)
	{
		ImGui::EndChild();
	}

	return retVal;
}


void gui::guiChild::EndUI()
{
    ImGui::EndChild();
}
