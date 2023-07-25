#pragma once
#include "Entity.h"

#include "Components.h"
#include "Transform.h"
namespace mh
{
	class GameObject : public Entity
	{
	public:
		enum class eState
		{
			Active,
			Paused,
			Dead,
		};

	public:
		GameObject();
		virtual ~GameObject();

		//DebugObject에서 재정의해서 사용 중
		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		Transform& GetTransform() { return mTransform; }

		template <typename T>
		T* AddComponent();
		
		void AddComponent(IComponent* _component);

		template <typename T>
		T* GetComponent();

		const std::vector<IComponent*>& GetComponents() { return mVecComponent; }
		
		const std::vector<IScript*>& GetScripts() { return mScripts; }

		void SetName(const std::string_view _Name) { mName = _Name; }
		const std::string& GetName() const { return mName; }

	public:
		bool IsDead()
		{
			if (mState == eState::Dead)
				return true;
			
			return false;
		}
		eState GetState() { return mState; }
		
		void Pause() { mState = eState::Paused; }
		void Death() { mState = eState::Dead; }
				
		bool IsDontDestroy() { return mbDontDestroy; }
		void DontDestroy(bool _enable) { mbDontDestroy = _enable; }
		
		define::eLayerType GetLayerType() { return mType; }
		void SetLayerType(define::eLayerType _type) { mType = _type; }

	private:
		Transform mTransform;
		std::vector<IComponent*> mVecComponent;

		std::string mName;
		eState mState;
		define::eLayerType mType;
		std::vector<IScript*> mScripts;
		bool mbDontDestroy;
	};

	template <typename T>
	T* GameObject::AddComponent()
	{
		T* _Comp = new T();
		define::eComponentType order = _Comp->GetOrder();

		if (order != define::eComponentType::Scripts)
		{
			mVecComponent[(UINT)order] = _Comp;
			mVecComponent[(UINT)order]->SetOwner(this);
		}
		else
		{
			mScripts.push_back(dynamic_cast<IScript*>(_Comp));
			_Comp->SetOwner(this);
		}

		_Comp->Initialize();

		return _Comp;
	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		T* castResult;
		for (auto component : mVecComponent)
		{
			castResult = dynamic_cast<T*>(component);

			if (castResult != nullptr)
				return castResult;
		}

		return nullptr;
	}

	//template <typename T>
	//std::vector<T*> GameObject::GetComponents()
	//{
	//	std::vector<T*> components = {};

	//	T* castResult;
	//	for (auto component : mVecComponent)
	//	{
	//		castResult = dynamic_cast<T*>(component);

	//		if (castResult != nullptr)
	//			components.push_back(castResult);
	//	}

	//	return components;
	//}
}

