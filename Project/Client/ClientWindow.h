#pragma once
#include "PCH_Client.h"



struct ClientWindowDesc
{
	HINSTANCE Inst;
	int nCmdShow;
	const TCHAR* TitleName;
	const TCHAR* ClassName;
	int SizeX;
	int SizeY;
};

struct tUmap_LightHashFunc32
{
	UINT64 operator()(const UINT32& _ukey) const
	{
		return static_cast<UINT64>(_ukey);
	}
};

using MsgHandleFunc = std::function<LRESULT WINAPI(HWND, UINT, WPARAM, LPARAM)>;
class ClientWindow
{

public:
	static void Init(ClientWindowDesc& _Desc);
	static inline void RegisterAdditionalWndProc(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> _Func);
	static void AddMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc);
	static void RemoveMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc);

	static HWND GetHwnd() { return mHwnd; }

private:
	static void DebugCheck();
	//_CrtSetBreakAlloc(652);
	static ATOM RegisterClientClass(ClientWindowDesc& _Desc);
	static BOOL InitInstance(ClientWindowDesc& _Desc);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	static LRESULT WINAPI Wm_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI Wm_Paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// 정보 대화 상자의 메시지 처리기입니다.


private:
	static HINSTANCE mInstance;
	static HWND mHwnd;

	
	static std::unordered_map<UINT, std::vector<MsgHandleFunc>, tUmap_LightHashFunc32> mMsgHandleFuncs;
	static std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> mImGuiWndProc;

private:
	ClientWindow() = delete;
	~ClientWindow() = delete;
};



inline void ClientWindow::RegisterAdditionalWndProc(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> _Func)
{
	mImGuiWndProc = _Func;
}

inline void ClientWindow::AddMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc)
{
	if (nullptr == _HandleFunc)
		return;

	mMsgHandleFuncs[_Message].push_back(_HandleFunc);
}


