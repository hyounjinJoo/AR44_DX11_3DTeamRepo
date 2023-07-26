#include "EnginePCH.h"

#include "GameObject.h"
#include "Com_Transform.h"

namespace mh
{
	GameObject::GameObject()
		: mTransform()
		, mFixedComponents()
		, mState(eState::Active)
		, mLayerType(define::eLayerType::None)
		, mbDontDestroy()
		, mName()
	{
		mFixedComponents.resize((int)eComponentType::Scripts);
		AddComponent(&mTransform);
	}


	GameObject::GameObject(const GameObject& _other)
		: Entity(_other)
		, mTransform(_other.mTransform)
		, mFixedComponents()
		, mState(_other.mState)
		, mLayerType(_other.mLayerType)
		, mbDontDestroy(_other.mbDontDestroy)
		, mName(_other.mName)
	{
		mFixedComponents.resize((int)eComponentType::Scripts);
		AddComponent(&mTransform);

		//TODO: Clone
		//1. 컴포넌트 목록 복사
		for (size_t i = 0; i < _other.mFixedComponents.size(); ++i)
		{
			if (_other.mFixedComponents[i])
			{
				//AddComponent(_other.mFixedComponents[i]->Clone());
			}
		}

		//2. 자녀 오브젝트 복사
		for (size_t i = 0; i < _other.mFixedComponents.size(); ++i)
		{
			//AddChildGameObj(_other.mFixedComponents[i]->Clone());
		}
	}

	GameObject::~GameObject()
	{
		//Transform은 제거 X
		for (size_t i = 1; i < mFixedComponents.size(); ++i)
		{
			if (nullptr == mFixedComponents[i])
				continue;
			delete mFixedComponents[i];
		}

		for (size_t i = 0; i < mScripts.size(); ++i)
		{
			if (nullptr == mScripts[i])
				continue;
			delete mScripts[i];
		}
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (nullptr == mChilds[i])
				continue;
			delete mChilds[i];
		}
	}
	
	void GameObject::Initialize()
	{
		for (size_t i = 0; i < mFixedComponents.size(); ++i)
		{
			if (nullptr == mFixedComponents[i])
				continue;
			mFixedComponents[i]->Initialize();
		}
		for (size_t i = 0; i < mScripts.size(); ++i)
		{
			if (nullptr == mScripts[i])
				continue;
			mScripts[i]->Initialize();
		}
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (nullptr == mChilds[i])
				continue;
			mChilds[i]->Initialize();
		}
	}

	void GameObject::Update()
	{
		for (size_t i = 0; i < mFixedComponents.size(); ++i)
		{
			if (nullptr == mFixedComponents[i])
				continue;
			mFixedComponents[i]->Update();
		}
		for (size_t i = 0; i < mScripts.size(); ++i)
		{
			if (nullptr == mScripts[i])
				continue;
			mScripts[i]->Update();
		}
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (nullptr == mChilds[i])
				continue;
			mChilds[i]->Update();
		}
	}

	void GameObject::FixedUpdate()
	{
		for (size_t i = 0; i < mFixedComponents.size(); ++i)
		{
			if (nullptr == mFixedComponents[i])
				continue;
			mFixedComponents[i]->FixedUpdate();
		}
		for (size_t i = 0; i < mScripts.size(); ++i)
		{
			if (nullptr == mScripts[i])
				continue;
			mScripts[i]->FixedUpdate();
		}
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (nullptr == mChilds[i])
				continue;
			mChilds[i]->FixedUpdate();
		}
	}

	void GameObject::Render()
	{
		for (size_t i = 0; i < mFixedComponents.size(); ++i)
		{
			if (nullptr == mFixedComponents[i])
				continue;
			mFixedComponents[i]->Render();
		}
		for (size_t i = 0; i < mScripts.size(); ++i)
		{
			if (nullptr == mScripts[i])
				continue;
			mScripts[i]->Render();
		}
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (nullptr == mChilds[i])
				continue;
			mChilds[i]->Render();
		}
	}



	IComponent* GameObject::AddComponent(IComponent* _pCom)
	{
		if (nullptr == _pCom)
			return nullptr;

		eComponentType ComType = _pCom->GetComType();

		if (_pCom->GetKey().empty())
		{
			ERROR_MESSAGE_W(
				LR"(
컴포넌트에 String Key가 없습니다.
new를 써서 만들지 말고, 
AddComponent<T> 또는 ComMgr::GetNewComponent()를 통해서 생성하세요.
)");
			MH_ASSERT(false == _pCom->GetKey().empty());
			SAFE_DELETE(_pCom);
			return nullptr;
		}

		if (eComponentType::Scripts == ComType)
		{
			mScripts.push_back(static_cast<IScript*>(_pCom));
		}
		else
		{
			if (nullptr != mFixedComponents[(int)ComType])
			{
				SAFE_DELETE(_pCom);
				ERROR_MESSAGE_W(L"이미 중복된 타입의 컴포넌트가 들어가 있습니다.");
				MH_ASSERT(nullptr);
				return nullptr;
			}

			mFixedComponents[(int)ComType] = _pCom;
		}


		_pCom->SetOwner(this);
		return _pCom;
	}
}
