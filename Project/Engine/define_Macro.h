#pragma once

#define STRKEY constexpr const char*
#define STRKEY_DECLARE(_AStr) STRKEY _AStr = #_AStr

#ifdef _DEBUG
#define DEBUG_BREAK DebugBreak()
#else
#define DEBUG_BREAK
#endif

#define ERROR_MESSAGE_A(_aStrMessage) MessageBoxA(nullptr, _aStrMessage, NULL, MB_OK); DEBUG_BREAK
#define ERROR_MESSAGE_W(_wStrMessage) MessageBoxW(nullptr, _wStrMessage, NULL, MB_OK); DEBUG_BREAK

#define MH_ASSERT(_Test) assert(_Test)

#define BIT_MASK(_MaskPos) 1<<_MaskPos
