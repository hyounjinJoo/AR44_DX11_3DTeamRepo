#include "PCH_Engine.h"
#include "SceneMgr.h"

#include "Com_Camera.h"
#include "Script_Camera.h"

#include "GridScript.h"

#include "Com_Renderer_Mesh.h"

#include "Script_Player.h"

#include "RenderMgr.h"
#include "ResMgr.h"


#include "Com_Renderer_Sprite.h"

#include "Com_Transform.h"
#include "Texture.h"

#include "AtExit.h"
namespace mh
{
	std::unique_ptr<IScene> SceneMgr::mActiveScene = nullptr;

	std::unordered_map<std::string_view, std::function<IScene* ()>> SceneMgr::mUmapSceneConstructor{};

	void SceneMgr::Init()
	{
		AtExit::AddFunc(Release);
	}

	void SceneMgr::Update()
	{
		if(mActiveScene)
			mActiveScene->SceneUpdate();
	}

	void SceneMgr::FixedUpdate()
	{
		if (mActiveScene)
			mActiveScene->SceneFixedUpdate();
	}

	void SceneMgr::Render()
	{
		if (mActiveScene)
			mActiveScene->SceneRender();
	}
	
	void SceneMgr::Destroy()
	{
		if (mActiveScene)
			mActiveScene->SceneDestroy();
	}

	void SceneMgr::Release()
	{
		mActiveScene.reset();
		mUmapSceneConstructor.clear();
	}


	void SceneMgr::LoadScene(const std::string_view _strKey)
	{
		const auto& Func = mUmapSceneConstructor.find(_strKey);

		if (Func == mUmapSceneConstructor.end())
		{
			ERROR_MESSAGE_W(L"그런 이름의 씬이 없습니다.");
			return;
		}

		std::unique_ptr<IScene> NewScene = std::unique_ptr<IScene>(Func->second());
		if (nullptr == NewScene)
		{
			ERROR_MESSAGE_W(L"씬 생성 실패");
			return;
		}

		// 바뀔때 dontDestory 오브젝트는 다음씬으로 같이 넘겨줘야한다.
		std::vector<GameObject*> gameObjs;

		if (mActiveScene)
		{
			gameObjs = mActiveScene->GetDontDestroyGameObjects();
		}
			

		if (mActiveScene)
			mActiveScene->OnExit();

		//씬 갈아끼우기
		mActiveScene.reset();
		mActiveScene = std::move(NewScene);

		
		//dontdestroy 오브젝트 추가
		for (GameObject* obj : gameObjs)
		{
			mActiveScene->AddGameObject(obj->GetLayerType(), obj);
		}

		//OnEnter 호출
		mActiveScene->OnEnter();

		mActiveScene->SceneInit();
	}
}
