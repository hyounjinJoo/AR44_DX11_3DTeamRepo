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


#include "ClientWindow.h"

#define MAX_LOADSTRING (100)

mh::Application     gApplication;
gui::Editor         gEditor;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    ClientWindowDesc Desc;
    Desc.Inst = hInstance;
    Desc.nCmdShow = nCmdShow;
    Desc.TitleName = TEXT("Monster Hunter World");
    Desc.ClassName = TEXT("GameEngine Client");
    Desc.SizeX = 1280;
    Desc.SizeY = 720;
    ClientWindow::Init(Desc);

    gApplication.SetWindow(ClientWindow::GetHwnd(), Desc.SizeX, Desc.SizeY);
    gApplication.Init();
    gEditor.Init();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
    MSG msg;
    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            gApplication.Run();
            gEditor.Run();
            gApplication.Present();
        }
    }


   
    mh::SceneManager::Release();

    AtExit::CallAtExit();
    gApplication.Release();
    gEditor.Release();


    return (int) msg.wParam;
}




//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


