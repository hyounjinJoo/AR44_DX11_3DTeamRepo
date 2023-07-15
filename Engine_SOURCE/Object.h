#pragma once
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Transform.h"

namespace mh::object
{
	template <typename T>
	static T* Instantiate(enums::eLayerType _type)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(gameObject);
		gameObject->Initalize();


		return gameObject;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _type, Scene* _scene)
	{
		T* gameObject = new T();
		Layer& layer = _scene->GetLayer(_type);
		layer.AddGameObject(gameObject);

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _type, Transform* _parent)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(gameObject);

		Transform* tr = gameObject->GameObject::GetComponent<Transform>();
		tr->SetParent(_parent);

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _type, Vector3 _position, Vector3 _rotation)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(gameObject);

		Transform* tr = gameObject->GameObject::GetComponent<Transform>();
		tr->SetPosition(_position);
		tr->SetRotation(_rotation);

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType _type, Vector3 _position, Vector3 _rotation, Transform* _parent)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetActiveScene();
		Layer& layer = scene->GetLayer(_type);
		layer.AddGameObject(gameObject);

		Transform* tr = gameObject->GameObject::GetComponent<Transform>();
		tr->SetPosition(_position);
		tr->SetRotation(_rotation);
		tr->SetParent(_parent);

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