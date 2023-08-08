#include "PCH_Client.h"
#include "GameClient.h"

#include <Engine/define_Macro.h>
#include <Engine/AtExit.h>

#include <Engine/Application.h>

#include <Contents/ContentsClassInitializer.h>

#include "ImGuiMgr.h"

HINSTANCE GameClient::mInstance;
HWND GameClient::mHwnd;
HACCEL GameClient::mHAccelTable;

std::unordered_map<UINT, std::vector<MsgHandleFunc>, tUmap_LightHashFunc32> GameClient::mMsgHandleFuncs;
std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> GameClient::mImGuiWndProc;

BOOL GameClient::Run(const GameClientDesc& _Desc)
{
    DebugCheck(0);

    BOOL bResult = FALSE;
    bResult = Init(_Desc);

    gui::ImGuiMgr::SetEnable(_Desc.RunEditor);

    while (bResult)
    {
        bResult = Loop();
    }

    Release();
    return bResult;
}


BOOL GameClient::Init(const GameClientDesc& _Desc)
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
    mh::ContentsClassInitializer::Init();

    mHAccelTable = LoadAccelerators(mInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
    return true;
}

BOOL GameClient::Loop()
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

            gui::ImGuiMgr::Run();
            
            mh::Application::Present();
        }
    }

    return bReturn;
}

void GameClient::Release()
{
    AtExit::CallAtExit();

    mInstance = {};
    mHwnd = {};

    mImGuiWndProc = nullptr;
    mMsgHandleFuncs.clear();
}

void GameClient::DebugCheck(long _block)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    if (_block)
    {
        _CrtSetBreakAlloc(_block);
    }
}

ATOM GameClient::RegisterClientClass(const GameClientDesc& _Desc)
{
    WNDCLASSEX WinClass;

    WinClass.cbSize = sizeof(WNDCLASSEX);

    WinClass.style = CS_HREDRAW | CS_VREDRAW;
    WinClass.lpfnWndProc = WndProc;
    WinClass.cbClsExtra = 0;
    WinClass.cbWndExtra = 0;
    WinClass.hInstance = mInstance;
    WinClass.hIcon = LoadIcon(mInstance, MAKEINTRESOURCE(IDI_ICON1));
    WinClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WinClass.lpszMenuName = NULL/*MAKEINTRESOURCEW(IDC_MY44ENGINE)*/;
    WinClass.lpszClassName = _Desc.ClassName;
    WinClass.hIconSm = LoadIcon(mInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassEx(&WinClass);
}

BOOL GameClient::InitInstance(const GameClientDesc& _Desc)
{
    mHwnd = CreateWindowW(_Desc.ClassName, _Desc.TitleName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _Desc.Inst, nullptr);
    if (!mHwnd)
    {
        return FALSE;
    }
    return TRUE;
}

LRESULT CALLBACK GameClient::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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



LRESULT __stdcall GameClient::Wm_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId = LOWORD(wParam);
    // 메뉴 선택을 구문 분석합니다:
    switch (wmId)
    {
    case IDM_ABOUT:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 1;
}

LRESULT __stdcall GameClient::Wm_Paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
    EndPaint(hWnd, &ps);

    return 1;
}

LRESULT __stdcall GameClient::Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 1;
}




void GameClient::RemoveMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc)
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
