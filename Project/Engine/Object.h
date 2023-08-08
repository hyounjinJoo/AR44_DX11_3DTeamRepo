#pragma once
#include "IScene.h"
#include "Layer.h"
#include "GameObject.h"
#include "SceneMgr.h"
#include "Com_Transform.h"

namespace mh::object
{
	inline void Instantiate(define::eLayerType _type, GameObject* _pObj)
	{
		IScene* scene = SceneMgr::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(_pObj);
		_pObj->Init();
	}

	template <typename T>
	static T* Instantiate(define::eLayerType _type)
	{
		T* gameObject = new T();
		IScene* scene = SceneMgr::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(gameObject);
		gameObject->Init();


		return gameObject;
	}

	template <typename T>
	static T* Instantiate(define::eLayerType _type, IScene* _scene)
	{
		T* gameObject = new T();
		Layer& layer = _scene->GetLayer(_type);
		layer.AddGameObject(gameObject);

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(define::eLayerType _type, Com_Transform* _parent)
	{
		T* gameObject = new T();
		IScene* scene = SceneMgr::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(gameObject);

		Com_Transform& tr = gameObject->GameObject::GetTransform();
		tr.SetOwner(_parent);

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(define::eLayerType _type, float3 _position, float3 _rotation)
	{
		T* gameObject = new T();
		IScene* scene = SceneMgr::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(gameObject);

		Com_Transform& tr = gameObject->GameObject::GetTransform();
		tr.SetPosition(_position);
		tr.SetRotation(_rotation);

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(define::eLayerType _type, float3 _position, float3 _rotation, Com_Transform* _parent)
	{
		T* gameObject = new T();
		IScene* scene = SceneMgr::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(gameObject);

		Com_Transform& tr = gameObject->GameObject::GetTransform();
		tr.SetPosition(_position);
		tr.SetRotation(_rotation);
		tr.SetOwner(_parent);

		return gameObject;
	}

	static void Destroy(GameObject* _gameObject)
	{
		_gameObject->Death();
	}

	static void DontDestroyOnLoad(GameObject* _gameObject)   //씬 이동시 이 오브젝트는 삭제하지 않는다
	{
		if (_gameObject == nullptr)
			return;

		_gameObject->DontDestroy(true);
	}
}
