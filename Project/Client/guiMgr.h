#pragma once
#include "guiChild.h"
#include <Engine/define_GPU.h>
#include <Engine/define_Struct.h>
#include <Engine/define_Macro.h>

namespace gui
{
	class EditorObject;
	class DebugObject;
	class guiMgr
	{
		friend class GameClient;
	public:
		static void Run();

		static inline void SetEnable(bool _bEnable);
		static inline bool GetEnable() { return mbEnable; }

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

	private:
		static void ImGuiInitialize();
		static void ImGuiRelease();

		static void ImGuiNewFrame();
		static void ImGuiRender();
		
		static void AddGuiWindow(guiBase* _pBase);

	private:
		static std::unordered_map<std::string, guiBase*, mh::define::tUmap_StringViewHasher, std::equal_to<>> mGuiWindows;

		static std::vector<EditorObject*> mEditorObjects;
		static std::vector<DebugObject*> mDebugObjects;

		static bool mbEnable;
		static bool mbInitialized;
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
		AddGuiWindow(static_cast<guiBase*>(retPtr));
		return retPtr;
	}
}

