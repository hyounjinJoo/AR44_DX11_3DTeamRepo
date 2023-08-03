#pragma once
#include "Entity.h"

#include "Components.h"
#include "ComMgr.h"

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

		GameObject(const GameObject& _other);
		CLONE(GameObject);

		virtual ~GameObject();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;
		
		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		Com_Transform& GetTransform() { return mTransform; }

		template <typename T>
		inline T* AddComponent();
		
		IComponent* AddComponent(IComponent* _pCom);
		inline IComponent* AddComponent(const std::string_view _strKey);

		template <typename T>
		inline T* GetComponent();

		template <typename T>
		inline eComponentType GetComponentType();

		const std::vector<IComponent*>& GetComponents() { return mComponents; }
		const std::vector<IScript*>& GetScripts() { return mScripts; }

		void SetName(const std::string_view _Name) { mName = _Name; }
		const std::string& GetName() const { return mName; }

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
		
		define::eLayerType GetLayerType() { return mLayerType; }
		void SetLayerType(define::eLayerType _type) { mLayerType = _type; }

		void AddChild(GameObject* _pObj);

	private:
		std::string mName;
		eState mState;
		define::eLayerType mLayerType;
		bool mbDontDestroy;

		Com_Transform mTransform;
		std::vector<IComponent*>	mComponents;
		std::vector<IScript*>		mScripts;

		GameObject* mParent;
		std::vector<GameObject*> mChilds;
	};


	template <typename T>
	T* GameObject::AddComponent()
	{
		define::eComponentType order = GetComponentType<T>();

		if (eComponentType::UNKNOWN == order)
			return nullptr;

		T* pCom = nullptr;
		if constexpr (std::is_base_of_v<IScript, T>)
		{
			pCom = new T;
			pCom->SetKey(ComMgr::GetComName<T>());
			mComponents.push_back(pCom);
			mScripts.push_back(pCom);
		}
		else
		{
			//타입을 알 수 없거나 이미 그쪽에 컴포넌트가 들어가 있을 경우 생성 불가
			if (eComponentType::UNKNOWN == order || nullptr != mComponents[(int)order])
				return nullptr;

			pCom = new T;
			pCom->SetKey(ComMgr::GetComName<T>());
			mComponents[(int)order] = pCom;
		}

		pCom->SetOwner(this);
		return pCom;
	}

	inline IComponent* GameObject::AddComponent(const std::string_view _strKey)
	{
		IComponent* pCom = ComMgr::GetNewCom(_strKey);

		if (nullptr == pCom)
		{
			return nullptr;
		}

		return AddComponent(pCom);
	}

	inline void GameObject::AddChild(GameObject* _pObj)
	{
		MH_ASSERT(false);
	}


	template <typename T>
	T* GameObject::GetComponent()
	{
		if constexpr (std::is_base_of_v<IScript, T>)
		{
			const std::string_view name = ComMgr::GetComName<T>();
			for (size_t i = 0; i < mScripts.size(); ++i)
			{
				if (name == mScripts[i]->GetKey())
					return static_cast<T*>(mScripts[i]);
			}
		}
		else
		{
			eComponentType ComType = GetComponentType<T>();
			return dynamic_cast<T*>(mComponents[(int)ComType]);
		}

		return nullptr;
	}


	template<typename T>
	inline eComponentType GameObject::GetComponentType()
	{
		if constexpr (std::is_base_of_v<Com_Transform, T>)
		{
			return eComponentType::Transform;
		}
		else if constexpr (std::is_base_of_v<ICollider, T>)
		{
			return eComponentType::Collider;
		}
		else if constexpr (std::is_base_of_v<Com_Animator, T>)
		{
			return eComponentType::Animator;
		}
		else if constexpr (std::is_base_of_v<Com_Light, T>)
		{
			return eComponentType::Light;
		}
		else if constexpr (std::is_base_of_v<Com_Camera, T>)
		{
			return eComponentType::Camera;
		}
		else if constexpr (std::is_base_of_v<Com_AudioSource, T>)
		{
			return eComponentType::AudioSource;
		}
		else if constexpr (std::is_base_of_v<Com_AudioListener, T>)
		{
			return eComponentType::AudioListener;
		}
		else if constexpr (std::is_base_of_v<IRenderer, T>)
		{
			return eComponentType::Renderer;
		}
		else if constexpr (std::is_base_of_v<IScript, T>)
		{
			return eComponentType::Scripts;
		}

		return eComponentType::UNKNOWN;
	}
}

