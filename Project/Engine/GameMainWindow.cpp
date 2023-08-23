#include "PCH_Engine.h"
#include "GameMainWindow.h"
#include "resource.h"

#include "define_Macro.h"
#include "AtExit.h"
         
#include "Application.h"

#include "guiMgr.h"

HINSTANCE GameMainWindow::mInstance{};
HWND GameMainWindow::mHwnd{};
HACCEL GameMainWindow::mHAccelTable{};

std::unordered_map<UINT, std::vector<MsgHandleFunc>, tUmap_LightHashFunc32> GameMainWindow::mMsgHandleFuncs{};
std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> GameMainWindow::mImGuiWndProc{};

BOOL GameMainWindow::Run(const tDesc_GameMainWindow& _Desc)
{

    BOOL bResult = FALSE;
    bResult = Init(_Desc);

    while (bResult)
    {
        bResult = Loop();
    }

    Release();
    return bResult;
}


BOOL GameMainWindow::Init(const tDesc_GameMainWindow& _Desc)
{
    mInstance = _Desc.Inst;
    AddMsgHandleFunc(WM_COMMAND, Wm_Command);
    AddMsgHandleFunc(WM_PAINT, Wm_Paint);
    AddMsgHandleFunc(WM_DESTROY, Wm_Destroy);


    if (false == RegisterClientClass(_Desc))
    {
        ERROR_MESSAGE_W(L"창 생성 실패");
        std::abort();
        return false;
    }
    else if (FALSE == InitInstance(_Desc))
    {
        ERROR_MESSAGE_W(L"창 생성 실패");
        std::abort();
        return false;
    }
    ShowWindow(mHwnd, SW_HIDE);
    UpdateWindow(mHwnd);


    mh::define::tDesc_Application AppDesc;
    AppDesc.Hwnd = mHwnd;
    AppDesc.Height = _Desc.Height;
    AppDesc.Width = _Desc.Width;
    AppDesc.LeftWindowPos = _Desc.LeftPos;
    AppDesc.TopWindowPos = _Desc.TopPos;
    AppDesc.GPUDesc = _Desc.GPUDesc;

    mh::Application::Init(AppDesc);
   
    for (size_t i = 0; i < _Desc.ExternalInitFuncs.size(); ++i)
    {
        if(_Desc.ExternalInitFuncs[i])
            _Desc.ExternalInitFuncs[i]();
    }

    mHAccelTable = LoadAccelerators(mInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
    return true;
}

BOOL GameMainWindow::Loop()
{
    BOOL bReturn = TRUE;
    MSG msg;
    // 기본 메시지 루프입니다:
    while (bReturn)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
            {
                bReturn = FALSE;
                break;
            }
                

            if (!TranslateAccelerator(msg.hwnd, mHAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            bReturn = mh::Application::Run();
            mh::Application::Present();
        }
    }

    return bReturn;
}

ATOM GameMainWindow::RegisterClientClass(const tDesc_GameMainWindow& _Desc)
{
    WNDCLASSEX WinClass;

    WinClass.cbSize = sizeof(WNDCLASSEX);

    WinClass.style = CS_HREDRAW | CS_VREDRAW;
    WinClass.lpfnWndProc = WndProc;
    WinClass.cbClsExtra = 0;
    WinClass.cbWndExtra = 0;
    WinClass.hInstance = mInstance;
    WinClass.hIcon = _Desc.WindowIcon;
    WinClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WinClass.lpszMenuName = NULL/*MAKEINTRESOURCEW(IDC_MY44ENGINE)*/;
    WinClass.lpszClassName = _Desc.ClassName;
    WinClass.hIconSm = _Desc.WindowIcon;

    return RegisterClassEx(&WinClass);
}

void GameMainWindow::Release()
{
    AtExit::CallAtExit();

    mInstance = {};
    mHwnd = {};

    mImGuiWndProc = nullptr;
    mMsgHandleFuncs.clear();
}




BOOL GameMainWindow::InitInstance(const tDesc_GameMainWindow& _Desc)
{
    mHwnd = CreateWindowW(_Desc.ClassName, _Desc.TitleName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _Desc.Inst, nullptr);
    if (!mHwnd)
    {
        return FALSE;
    }
    return TRUE;
}

LRESULT CALLBACK GameMainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (mImGuiWndProc)
    {
        LRESULT result = mImGuiWndProc(hWnd, message, wParam, lParam);
        if (result)
        {
            return result;
        }
    }


    const auto& iter = mMsgHandleFuncs.find(message);
    if (iter != mMsgHandleFuncs.end())
    {
        for (size_t i = 0; i < iter->second.size(); ++i)
        {
            if (iter->second[i])
            {
                LRESULT result = iter->second[i](hWnd, message, wParam, lParam);
                if (result)
                    return result;
            }
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}



LRESULT __stdcall GameMainWindow::Wm_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId = LOWORD(wParam);
    // 메뉴 선택을 구문 분석합니다:
    //switch (wmId)
    //{
    //default:
    //    return DefWindowProc(hWnd, message, wParam, lParam);
    //}

    return 1;
}

LRESULT __stdcall GameMainWindow::Wm_Paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
    EndPaint(hWnd, &ps);

    return 1;
}

LRESULT __stdcall GameMainWindow::Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 1;
}




void GameMainWindow::RemoveMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc)
{
    const auto& mapIter = mMsgHandleFuncs.find(_Message);
    if (mapIter != mMsgHandleFuncs.end())
    {
        const auto& vec = mapIter->second;
        for (auto vecIter = vec.begin(); vec.end() != vecIter; ++vecIter)
        {
            if (vecIter->target<void(*)>() == _HandleFunc.target<void(*)>())
            {
                mapIter->second.erase(vecIter);
                break;
            }
        }
    }
}
