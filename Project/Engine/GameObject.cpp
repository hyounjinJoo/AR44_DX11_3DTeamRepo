#include "EnginePCH.h"

#include "GameObject.h"
#include "Transform.h"

namespace mh
{
	GameObject::GameObject()
		: mState(eState::Active)
		, mType(define::eLayerType::None)
		, mbDontDestroy(false)
	{
		mComponents.resize((UINT)define::eComponentType::End);
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



	void GameObject::AddComponent(IComponent* _Comp)
	{
		define::eComponentType order = _Comp->GetOrder();

		if (order != define::eComponentType::Script)
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



}
