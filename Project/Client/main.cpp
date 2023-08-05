#include "PCH_Client.h"

// 44Engine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <Engine/Application.h>
#include <Engine/SceneManager.h>
#include <Engine/AtExit.h>

#include "guiEditor.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


#include "GameClient.h"

gui::EditorMgr         gEditor;


BOOL APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    GameClientDesc Desc{};

    Desc.Inst = hInstance;
    Desc.TitleName = TEXT("Monster Hunter World");
    Desc.ClassName = TEXT("GameEngine Client");
    Desc.LeftPos = 100;
    Desc.TopPos = 100;
    Desc.Width = 1280;
    Desc.Height = 720;
    Desc.RunEditor = true;

    Desc.GPUDesc.ResolutionX = (UINT)Desc.Width;
    Desc.GPUDesc.ResolutionY = (UINT)Desc.Height;
    Desc.GPUDesc.RefreshRate = 144u;

    int a = 123;

    return GameClient::Run(Desc);
}





