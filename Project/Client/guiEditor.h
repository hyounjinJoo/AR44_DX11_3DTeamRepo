#pragma once
#include "guiDebugObject.h"
#include "guiEditorObject.h"
#include "guiWidget.h"
#include <Engine/define_GPU.h>

namespace gui
{
	class EditorMgr
	{
		friend class GameClient;
	public:
		static void Run();

		static inline void SetEnable(bool _bEnable);
		static inline bool GetEnable() { return mbEnable; }

		template<typename T>
		T* GetWidget(const std::string& name);


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
		static std::map<std::string, Widget*> mWidgets;
		static std::vector<EditorObject*> mEditorObjects;
		static std::vector<DebugObject*> mDebugObjects;

		static class YamYamEditor* mYamYamEditor;
		static bool mbEnable;
		static bool mbInitialized;

	};



	inline void EditorMgr::SetEnable(bool _bEnable)
	{
		mbEnable = _bEnable;

		if (mbEnable && (false == mbInitialized))
		{
			mbInitialized = true;
			EditorMgr::Init();
		}
	}

	template<typename T>
	inline T* EditorMgr::GetWidget(const std::string& name)
	{
		auto iter = mWidgets.find(name);
		if (iter == mWidgets.end())
			return nullptr;

		return dynamic_cast<T*>(iter->second);
	}
}

