#pragma once
#include "guiDebugObject.h"
#include "guiEditorObject.h"
#include "guiWidget.h"
#include <Engine/define_GPU.h>
#include <Engine/define_Struct.h>

namespace gui
{
	class ImGuiMgr
	{
		friend class GameClient;
	public:
		static void Run();

		static inline void SetEnable(bool _bEnable);
		static inline bool GetEnable() { return mbEnable; }

		template<typename T>
		static T* GetWidget(const std::string& name);

		static const std::unordered_map<std::string, Widget*, mh::define::tUmap_StringViewHasher, std::equal_to<>>&
			GetWidgets() { return mWidgets; }


	private:
		static void Init();
		static void Release();

		static void Update();
		static void FixedUpdate();
		static void Render();
		
		static void DebugRender(mh::tDebugMesh& mesh);

		static void ImGuiInitialize();
		static void ImGuiRun();
		static void ImGuiRelease();



	private:
		static std::unordered_map<std::string, Widget*, mh::define::tUmap_StringViewHasher, std::equal_to<>> mWidgets;

		static std::vector<EditorObject*> mEditorObjects;
		static std::vector<DebugObject*> mDebugObjects;

		static class MainMenu* mEditor;
		static bool mbEnable;
		static bool mbInitialized;

	};



	inline void ImGuiMgr::SetEnable(bool _bEnable)
	{
		mbEnable = _bEnable;

		if (mbEnable && (false == mbInitialized))
		{
			mbInitialized = true;
			ImGuiMgr::Init();
		}
	}

	template<typename T>
	inline T* ImGuiMgr::GetWidget(const std::string& name)
	{
		auto iter = mWidgets.find(name);
		if (iter == mWidgets.end())
			return nullptr;

		return dynamic_cast<T*>(iter->second);
	}
}

