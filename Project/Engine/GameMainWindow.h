#pragma once

#include <Windows.h>
#include <vector>
#include <functional>

#include "define_Struct.h"

struct tDesc_GameMainWindow
{
	HINSTANCE Inst;
	HICON	  WindowIcon;
	const TCHAR* TitleName;
	const TCHAR* ClassName;
	int LeftPos;
	int TopPos;
	int Width;
	int Height;

	std::vector<std::function<void()>> ExternalInitFuncs;

	mh::define::tDesc_GPUMgr GPUDesc;

	bool RunEditor;
};

struct tUmap_LightHashFunc32
{
	UINT64 operator()(const UINT32& _ukey) const
	{
		return static_cast<UINT64>(_ukey);
	}
};

using MsgHandleFunc = std::function<LRESULT WINAPI(HWND, UINT, WPARAM, LPARAM)>;
class GameMainWindow
{
public:
	static BOOL Run(const tDesc_GameMainWindow& _Desc);

	static inline void RegisterImGuiWndProc(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> _Func);
	static void AddMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc);
	static void RemoveMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc);
	
private:
	static BOOL Init(const tDesc_GameMainWindow& _Desc);
	static BOOL Loop();
	static ATOM RegisterClientClass(const tDesc_GameMainWindow& _Desc);
	static BOOL InitInstance(const tDesc_GameMainWindow& _Desc);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void Release();

	static void DebugCheck(long _block);
	static LRESULT WINAPI Wm_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI Wm_Paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HINSTANCE mInstance;
	static HWND mHwnd;
	static HACCEL mHAccelTable;


	static std::unordered_map<UINT, std::vector<MsgHandleFunc>, tUmap_LightHashFunc32> mMsgHandleFuncs;
	static std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> mImGuiWndProc;

private:
	GameMainWindow() = delete;
	~GameMainWindow() = delete;
};



inline void GameMainWindow::RegisterImGuiWndProc(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> _Func)
{
	mImGuiWndProc = _Func;
}

inline void GameMainWindow::AddMsgHandleFunc(UINT _Message, MsgHandleFunc _HandleFunc)
{
	if (nullptr == _HandleFunc)
		return;

	mMsgHandleFuncs[_Message].push_back(_HandleFunc);
}


