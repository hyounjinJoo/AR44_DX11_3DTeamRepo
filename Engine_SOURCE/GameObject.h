#pragma once
#include "IComponent.h"

#include "Enums.h"
#include "Script.h"
#include "Entity.h"


namespace mh
{
	class GameObject : public Entity
	{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		enum class eState
		{
			Active,
			Paused,
			Dead,
		};

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
}

