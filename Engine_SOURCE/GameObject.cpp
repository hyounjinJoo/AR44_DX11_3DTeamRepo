#include "GameObject.h"
#include "Transform.h"

namespace mh
{
	GameObject::GameObject()
		: mState(eState::Active)
		, mType(enums::eLayerType::None)
		, mbDontDestroy(false)
	{
		mComponents.resize((UINT)enums::eComponentType::End);
		AddComponent(new Transform());
	}

	GameObject::~GameObject()
	{
		for (IComponent* component : mComponents)
		{
			if (component == nullptr)
				continue;

			delete component;
			component = nullptr;
		}

		for (IComponent* script : mScripts)
		{
			if (script == nullptr)
				continue;

			delete script;
			script = nullptr;
		}
	}

	void GameObject::Initialize()
	{

	}

	void GameObject::Update()
	{
		for (IComponent* component : mComponents)
		{
			if (component == nullptr)
				continue;

			component->Update();
		}

		for (IComponent* script : mScripts)
		{
			if (script == nullptr)
				continue;

			script->Update();
		}
	}

	void GameObject::FixedUpdate()
	{
		for (IComponent* component : mComponents)
		{
			if (component == nullptr)
				continue;

			component->FixedUpdate();
		}

		for (IComponent* script : mScripts)
		{
			if (script == nullptr)
				continue;

			script->FixedUpdate();
		}
	}

	void GameObject::Render()
	{
		for (IComponent* component : mComponents)
		{
			if (component == nullptr)
				continue;

			component->Render();
		}

		for (IComponent* script : mScripts)
		{
			if (script == nullptr)
				continue;

			script->Render();
		}
	}

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

	void GameObject::AddComponent(IComponent* _Comp)
	{
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
