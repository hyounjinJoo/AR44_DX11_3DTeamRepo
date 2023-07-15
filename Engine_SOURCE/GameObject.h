#pragma once
#include "IComponent.h"
#include "Script.h"
#include "Entity.h"

namespace mh
{
	enum eState
	{
		Active,
		Paused,
		Dead,
	};

	class GameObject : public Entity
	{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void Initalize();
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
		
		eLayerType GetLayerType() { return mType; }
		void SetLayerType(eLayerType _type) { mType = _type; }

	protected:
		std::vector<Component*> mComponents;

	private:
		eState mState;
		eLayerType mType;
		std::vector<Script*> mScripts;
		bool mbDontDestroy;
	};
}

