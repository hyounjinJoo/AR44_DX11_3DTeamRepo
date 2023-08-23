#pragma once
#include "IScene.h"
#include "Layer.h"
#include "GameObject.h"
#include "SceneMgr.h"
#include "Com_Transform.h"

namespace mh::object
{
	//inline GameObject* Instantiate(define::eLayerType _type, GameObject* _pObj)
	//{
	//	IScene* scene = SceneMgr::GetActiveScene();

	//	_pObj->AddToLayerRecursive(_type);

	//	if(scene->IsInitialized())
	//		_pObj->Init();

	//	return _pObj;
	//}


	//static void Destroy(GameObject* _gameObject)
	//{
	//	_gameObject->Death();
	//}

	//static void DontDestroyOnLoad(GameObject* _gameObject)   //씬 이동시 이 오브젝트는 삭제하지 않는다
	//{
	//	if (_gameObject == nullptr)
	//		return;

	//	_gameObject->DontDestroy(true);
	//}
}
