#pragma once
#include "guiDebugObject.h"
#include "guiEditorObject.h"
#include "guiWidget.h"
#include <Engine/define_GPU.h>

namespace gui
{
	class Editor
	{
	public:
		void Initialize();
		void Run();

		void Update();
		void FixedUpdate();
		void Render();
		void Release();
		void DebugRender(mh::tDebugMesh& mesh);

		void ImGuiInitialize();
		void ImGuiRun();
		void ImGuiRelease();

		template<typename T>
		T* GetWidget(const std::string& name)
		{
			auto iter = mWidgets.find(name);
			if (iter == mWidgets.end())
				return nullptr;

			return dynamic_cast<T*>(iter->second);
		}

	private:
		std::map<std::string, Widget*> mWidgets;
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;

		class YamYamEditor* mYamYamEditor;
		bool mbEnable = true;
	};
}
