#pragma once

#include "Enums.h"
#include "IComponent.h"
#include "Script.h"
#include "Entity.h"

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

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		template <typename T>
		T* AddComponent();
		
		void AddComponent(IComponent* _component);


		template <typename T>
		T* GetComponent();

		template <typename T>
		std::vector<T*> GetComponents();
		
		const std::vector<Script*>& GetScripts() { return mScripts; }

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
		
		enums::eLayerType GetLayerType() { return mType; }
		void SetLayerType(enums::eLayerType _type) { mType = _type; }

	protected:
		std::vector<IComponent*> mComponents;

	private:
		eState mState;
		enums::eLayerType mType;
		std::vector<Script*> mScripts;
		bool mbDontDestroy;
	};

	template <typename T>
	T* GameObject::AddComponent()
	{
		T* _Comp = new T();
		enums::eComponentType order = _Comp->GetOrder();

		if (order != enums::eComponentType::Script)
		{
			mComponents[(UINT)order] = _Comp;
			mComponents[(UINT)order]->SetOwner(this);
		}
		else
		{
			mScripts.push_back(dynamic_cast<Script*>(_Comp));
			_Comp->SetOwner(this);
		}

		_Comp->Initialize();

		return _Comp;
	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		T* castResult;
		for (auto component : mComponents)
		{
			castResult = dynamic_cast<T*>(component);

			if (castResult != nullptr)
				return castResult;
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GameObject::GetComponents()
	{
		std::vector<T*> components = {};

		T* castResult;
		for (auto component : mComponents)
		{
			castResult = dynamic_cast<T*>(component);

			if (castResult != nullptr)
				components.push_back(castResult);
		}

		return components;
	}
}

