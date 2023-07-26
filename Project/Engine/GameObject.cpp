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
		mVecComponent.resize((UINT)define::eComponentType::END);
		
		AddComponent(&mTransform);
	}

	GameObject::~GameObject()
	{
		//Transform을 제외하고 나머지는 동적할당 한 컴포넌트이므로 제거
		for (size_t i = 1; i < mVecComponent.size(); ++i)
		{
			if (nullptr == mVecComponent[i])
				continue;

			delete mVecComponent[i];
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
