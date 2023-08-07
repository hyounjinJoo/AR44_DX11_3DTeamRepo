
#include "PCH_Engine.h"

#include "Com_Camera.h"
#include "Script_Camera.h"

#include "GridScript.h"

#include "Com_Renderer_Mesh.h"

#include "Script_Player.h"
#include "PlayScene.h"

#include "RenderMgr.h"
#include "ResMgr.h"

#include "SceneMgr.h"
#include "Com_Renderer_Sprite.h"

#include "Com_Transform.h"
#include "TitleScene.h"
#include "Texture.h"

#include "AtExit.h"
namespace mh
{
	std::vector<Scene*> SceneMgr::mScenes = {};
	Scene* SceneMgr::mActiveScene = nullptr;

	void SceneMgr::Init()
	{
		AtExit::AddFunc(Release);

		mScenes.resize((uint)eSceneType::End);

		mScenes[(uint)eSceneType::Tilte] = new TitleScene();
		mScenes[(uint)eSceneType::Tilte]->SetKey("TitleScene");

		mScenes[(uint)eSceneType::Play] = new PlayScene();
		mScenes[(uint)eSceneType::Play]->SetKey("PlayScene");

		mActiveScene = mScenes[(uint)eSceneType::Tilte];

		for (Scene* scene : mScenes)
		{
			scene->Init();
		}
	}

	void SceneMgr::Update()
	{
		mActiveScene->Update();
	}

	void SceneMgr::FixedUpdate()
	{
		mActiveScene->FixedUpdate();
	}

	void SceneMgr::Render()
	{
		mActiveScene->Render();
	}
	
	void SceneMgr::Destroy()
	{
		mActiveScene->Destroy();
	}

	void SceneMgr::Release()
	{
		for (Scene* scene : mScenes)
		{
			delete scene;
			scene = nullptr;
		}
	}
	void SceneMgr::LoadScene(eSceneType _Type)
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		// 바뀔때 dontDestory 오브젝트는 다음씬으로 같이 넘겨줘야한다.
		std::vector<GameObject*> gameObjs 
			= mActiveScene->GetDontDestroyGameObjects();
		mActiveScene = mScenes[(uint)_Type];
		
		for (GameObject* obj : gameObjs)
		{
			eLayerType type = obj->GetLayerType();
			mActiveScene->AddGameObject(obj, type);
		}

		mActiveScene->OnEnter();
	}
}
