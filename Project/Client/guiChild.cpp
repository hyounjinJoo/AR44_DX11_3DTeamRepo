#include "PCH_Client.h"
#include "guiChild.h"

gui::guiChild::guiChild(const std::string_view _strKey)
    : guiWindow(_strKey)
    , mbBorder(true)
{
}

gui::guiChild::~guiChild()
{
}

bool gui::guiChild::BeginUI()
{
    return ImGui::BeginChild(GetKey().c_str(), ImVec2(0.f, 0.f), mbBorder, GetImGuiWindowFlag());
}


void gui::guiChild::EndUI()
{
    ImGui::EndChild();
}
