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

#define NOTIFICATION_W(_wStrMessage) MessageBoxW(nullptr, _wStrMessage, NULL, MB_OK)

#define MH_ASSERT(expression) assert(expression)

#define BIT_MASK(_MaskPos) 1<<_MaskPos

#define SAFE_DELETE(_Ptr) if(_Ptr) delete _Ptr; _Ptr = nullptr

#define CLONE(_type) virtual _type* Clone() override { return new _type(*this); }
#define CLONE_DISABLE(_type) virtual _type* Clone() override { ERROR_MESSAGE_W(L"Clone할 수 없거나 Clone함수를 재정의하지 않은 클래스입니다."); return nullptr; }


#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define GLOBAL_GRAVITY Application::GetPhysics()->GetGravity()
#define PHYSICS Application::GetPhysics()->GetPhysics()
#define GET_SINGLE(type) type::GetInstance()
