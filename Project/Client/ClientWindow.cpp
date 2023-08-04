#include "PCH_Client.h"
#include "ClientWindow.h"

#include <Engine/define_Macro.h>

HINSTANCE ClientWindow::mInstance;
std::unordered_map<UINT, std::vector<MsgHandleFunc>, tUmap_LightHashFunc32> ClientWindow::mMsgHandleFuncs;
std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> ClientWindow::mImGuiWndProc;
HWND ClientWindow::mHwnd;

void ClientWindow::Init(ClientWindowDesc& _Desc)
{
    mInstance = _Desc.Inst;
    DebugCheck();

    AddMsgHandleFunc(WM_COMMAND, Wm_Command);
    AddMsgHandleFunc(WM_PAINT, Wm_Paint);
    AddMsgHandleFunc(WM_DESTROY, Wm_Destroy);

    if (false == RegisterClientClass(_Desc))
    {
        ERROR_MESSAGE_W(L"창 생성 실패");
        std::abort();
    }
    else if (FALSE == InitInstance(_Desc))
    {
        ERROR_MESSAGE_W(L"창 생성 실패");
        std::abort();
    }

    ShowWindow(mHwnd, _Desc.nCmdShow);
    UpdateWindow(mHwnd);
}



void ClientWindow::DebugCheck()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

ATOM ClientWindow::RegisterClientClass(ClientWindowDesc& _Desc)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = mInstance;
    wcex.hIcon = LoadIcon(mInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL/*MAKEINTRESOURCEW(IDC_MY44ENGINE)*/;
    wcex.lpszClassName = _Desc.ClassName;
    wcex.hIconSm = LoadIcon(mInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

BOOL ClientWindow::InitInstance(ClientWindowDesc& _Desc)
{
    mHwnd = CreateWindowW(_Desc.ClassName, _Desc.TitleName, WS_OVERLAPPEDWINDOW,
        0, 0, _Desc.SizeX, _Desc.SizeY, nullptr, nullptr, _Desc.Inst, nullptr);
    if (!mHwnd)
    {
        return FALSE;
    }
    return TRUE;
}

LRESULT CALLBACK ClientWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
    //return 0;
}

LRESULT __stdcall ClientWindow::Wm_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId = LOWORD(wParam);
    // 메뉴 선택을 구문 분석합니다:
    switch (wmId)
    {
    case IDM_ABOUT:
        //DialogBox(mInstance, MAKEINTRESOURCE(IDM_ABOUT), hWnd, About);
        break;
    //case IDM_EXIT:
    //    DestroyWindow(hWnd);
    //    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 1;
}

LRESULT __stdcall ClientWindow::Wm_Paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
    EndPaint(hWnd, &ps);

    return 1;
}

LRESULT __stdcall ClientWindow::Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 1;
}


LRESULT __stdcall ClientWindow::About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void ClientWindow::RemoveMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc)
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
