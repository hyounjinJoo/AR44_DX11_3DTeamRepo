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
		mTransform.SetOwner(this);
		mVecComponent.resize((UINT)define::eComponentType::END);
		//AddComponent(new Transform());
	}

	GameObject::~GameObject()
	{
		for (IComponent* component : mVecComponent)
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
		//mTransform.Update();

		for (IComponent* component : mVecComponent)
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

		mTransform.FixedUpdate();
		for (IComponent* component : mVecComponent)
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
		for (IComponent* component : mVecComponent)
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
	}



}
