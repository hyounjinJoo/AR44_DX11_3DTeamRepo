#pragma once
#include "guiDebugObject.h"
#include "guiEditorObject.h"
#include "guiBase.h"
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
		static T* FindGUIWindow(const std::string_view _strKey);

		template <typename T>
		static T* AddGUIWindow(const std::string_view _strKey);

		static const std::vector<guiBase*>&
			GetGUIs() { return mGuiWindows; }

	private:
		static void Init();
		static void Release();

		static void Update();
		static void FixedUpdate();
		static void Render();
		
		static void DebugRender(mh::tDebugMesh& mesh);

	private:
		static void ImGuiInitialize();
		static void ImGuiNewFrame();
		static void ImGuiRender();
		static void ImGuiRelease();

	private:
		//static std::unordered_map<std::string, guiBase*, mh::define::tUmap_StringViewHasher, std::equal_to<>> mGuiWindows;

		static std::vector<guiBase*> mGuiWindows;

		static std::vector<EditorObject*> mEditorObjects;
		static std::vector<DebugObject*> mDebugObjects;

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

	

	template <typename T>
	inline T* ImGuiMgr::FindGUIWindow(const std::string_view _name)
	{
		T* ptr = nullptr;
		for (guiBase* iter : mGuiWindows)
		{
			if (_name == iter->GetKey())
			{
				ptr = dynamic_cast<T*>(iter);
				break;
			}
		}

		return ptr;
	}


	template<typename T>
	inline T* ImGuiMgr::AddGUIWindow(const std::string_view _strKey)
	{
		if (_strKey.empty())
			return nullptr;

		for (size_t i = 0; i < mGuiWindows.size(); ++i)
		{
			if (_strKey == mGuiWindows[i]->GetKey())
			{
				return nullptr;
			}
		}

		T* retPtr = new T;
		retPtr->SetKey(_strKey);
		
		if (mbInitialized)
		{
			retPtr->Init();
		}



		return retPtr;
	}
}

