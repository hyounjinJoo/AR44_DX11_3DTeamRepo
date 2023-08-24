#pragma once
#include "guiChild.h"
#include "define_GPU.h"
#include "define_Struct.h"
#include "define_Macro.h"
#include <unordered_map>
#include "json-cpp/json-forwards.h"

namespace gui
{
	class EditorObject;
	class DebugObject;
	class guiMgr
	{
		friend class GameEngineWindow;
	public:
		static void Run();

		static inline void SetEnable(bool _bEnable);
		static inline bool GetEnable() { return mbEnable; }
		static inline void ToggleEnable() { SetEnable(!mbEnable); }

		template<typename T>
		static T* FindGuiWindow(const std::string_view _strKey);

		//타입만으로도 검색 가능, 느림
		template <typename T>
		static T* FindGuiWindow();

		static guiBase* FindGuiWindow(const std::string_view _strKey);

		template <typename T>
		static T* AddGuiWindow();

		static const std::unordered_map<std::string, guiBase*, mh::define::tUmap_StringViewHasher, std::equal_to<>>&
			GetGUIs() { return mGuiWindows; }

	private:
		static void Init();
		static void Release();

		static void Update();
		static void FixedUpdate();
		static void Render();
		
		static void DebugRender(mh::define::tDebugMesh& mesh);

		static Json::Value* CheckJsonSaved(const std::string& _strKey);

		//=================== IMGUI ===========================
		static void ImGuiInitialize();
		static void ImGuiRelease();

		static void ImGuiNewFrame();
		static void ImGuiRender();
		
		static void AddGuiWindow(guiBase* _pBase);

	private:
		static std::unordered_map<std::string, guiBase*, mh::define::tUmap_StringViewHasher, std::equal_to<>> mGuiWindows;

		static std::vector<EditorObject*> mEditorObjects;
		static std::vector<DebugObject*> mDebugObjects;

		//현재 GUI를 표시할지 여부
		static bool mbEnable;

		//GUI가 최초 1회 초기화 되었는지 여부
		static bool mbInitialized;

		static std::unique_ptr<Json::Value> mJsonUIData;
	};

	inline void guiMgr::SetEnable(bool _bEnable)
	{
		mbEnable = _bEnable;

		if (mbEnable && (false == mbInitialized))
		{
			guiMgr::Init();
			mbInitialized = true;
		}
	}

	template <typename T>
	inline T* guiMgr::FindGuiWindow(const std::string_view _name)
	{
		return dynamic_cast<T*>(FindGuiWindow(_name));
	}

	template<typename T>
	inline T* guiMgr::FindGuiWindow()
	{
		T t{};
		const std::string& guiName = t.GetName();
		return FindGuiWindow<T>(guiName);
	}


	template<typename T>
	inline T* guiMgr::AddGuiWindow()
	{
		static_assert(std::is_base_of_v<guiBase, T>);

		//gui Child는 무조건 child로 들어가기위한 용도이므로 assert
		static_assert(false == std::is_base_of_v<guiChild, T>);

		T* retPtr = new T;

		Json::Value* pJval = CheckJsonSaved(retPtr->GetName());
		if (pJval)
		{
			retPtr->LoadJson(pJval);
		}

		AddGuiWindow(static_cast<guiBase*>(retPtr));
		return retPtr;
	}
}

