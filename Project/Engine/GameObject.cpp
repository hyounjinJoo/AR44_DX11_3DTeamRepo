#include "PCH_Engine.h"

#include "GameObject.h"
#include "Com_Transform.h"

#include "json-cpp/json.h"
#include "Prefab.h"

namespace mh
{
	namespace define::strKey::Json
	{
		namespace GameObject
		{
			STRKEY_DECLARE(mComponents);
			STRKEY_DECLARE(mChilds);
			//STRKEY_DECLARE(mScripts);
		}
	}


	GameObject::GameObject()
		: mTransform()
		, mComponents()
		, mState(eState::Active)
		, mLayerType(define::eLayerType::None)
		, mbDontDestroy()
		, mName()
	{
		mComponents.resize((int)eComponentType::Scripts);
		AddComponent(&mTransform);
	}


	GameObject::GameObject(const GameObject& _other)
		: Entity(_other)
		, mTransform(_other.mTransform)
		, mComponents()
		, mState(_other.mState)
		, mLayerType(_other.mLayerType)
		, mbDontDestroy(_other.mbDontDestroy)
		, mName(_other.mName)
	{
		mComponents.resize((int)eComponentType::Scripts);
		AddComponent(&mTransform);

		//TODO: Clone
		//1. 컴포넌트 목록 복사
		for (size_t i = 0; i < _other.mComponents.size(); ++i)
		{
			if (_other.mComponents[i])
			{
				//AddComponent(_other.mFixedComponents[i]->Clone());
			}
		}

		//2. 자녀 오브젝트 복사
		for (size_t i = 0; i < _other.mComponents.size(); ++i)
		{
			//AddChildGameObj(_other.mFixedComponents[i]->Clone());
		}
	}

	GameObject::~GameObject()
	{
		//Transform은 제거 X
		for (size_t i = 1; i < mComponents.size(); ++i)
		{
			if (nullptr == mComponents[i])
				continue;
			delete mComponents[i];
		}

		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (nullptr == mChilds[i])
				continue;
			delete mChilds[i];
		}
	}

	eResult GameObject::SaveJson(Json::Value* _pJson)
	{
		eResult Result = Entity::SaveJson(_pJson);
		if (eResultFail(Result))
		{
			return Result;
		}

		Json::MHSaveValue(_pJson, JSON_KEY_PAIR(mName));
		Json::MHSaveValue(_pJson, JSON_KEY_PAIR(mState));
		Json::MHSaveValue(_pJson, JSON_KEY_PAIR(mLayerType));
		Json::MHSaveValue(_pJson, JSON_KEY_PAIR(mbDontDestroy));

		{
			(*_pJson)[strKey::Json::GameObject::mComponents] = Json::Value(Json::arrayValue);
			Json::Value& arrComponent = (*_pJson)[strKey::Json::GameObject::mComponents];
			
			//트랜스폼은 저장하지 않음
			for (size_t i = (size_t)eComponentType::Transform + (size_t)1; i < mComponents.size(); ++i)
			{
				if (mComponents[i])
				{
					Json::Value ComJson = Json::Value(Json::objectValue);
					
					Result = mComponents[i]->SaveJson(&(ComJson));
					if (eResultFail(Result))
					{
						return Result;
					}
					arrComponent.append(ComJson);
				}
				else
				{
					arrComponent.append(Json::Value(Json::nullValue));
				}
			}
		}


		//GameObject* mParent;
		//부모 오브젝트가 있을 경우 재귀 구조로 부모 쪽에서 생성한 뒤 자식으로 등록할 것임

		//child의 경우 별도의 프리팹으로 취급해서 새로운 파일을 생성
		//std::vector<GameObject*> mChilds;
		{
			(*_pJson)[strKey::Json::GameObject::mChilds] = Json::Value(Json::arrayValue);
			Json::Value& arrChilds = (*_pJson)[strKey::Json::GameObject::mChilds];
			for (size_t i = 0; i < mChilds.size(); ++i)
			{
				if (mChilds[i])
				{
					//자식의 Key가 존재하지 않을 경우 자신의 Key에 숫자를 붙여서 생성
					std::string childStrKey = mChilds[i]->GetKey();
					if (childStrKey.empty())
					{
						childStrKey = GetKey() + "_";
						childStrKey += std::to_string((int)i);
						mChilds[i]->SetKey(childStrKey);
					}

					Prefab SavePrefab{};
					SavePrefab.RegisterPrefab(mChilds[i], true);
					eResult Result = SavePrefab.Save(childStrKey);
					if (eResultFail(Result))
					{
						return Result;
					}

					//자식 프리팹 이름을 등록
					arrChilds.append(childStrKey);
				}
			}
		}


		return eResult::Success;
	}

	eResult GameObject::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}

		eResult Result = Entity::LoadJson(_pJson);
		if (eResultFail(Result))
		{
			return Result;
		}

		Json::MHLoadValue(_pJson, JSON_KEY_PAIR(mName));
		Json::MHLoadValue(_pJson, JSON_KEY_PAIR(mState));
		Json::MHLoadValue(_pJson, JSON_KEY_PAIR(mLayerType));
		Json::MHLoadValue(_pJson, JSON_KEY_PAIR(mbDontDestroy));

		//컴포넌트 추가
		if (_pJson->isMember(strKey::Json::GameObject::mComponents))
		{
			const Json::Value& jValCom = (*_pJson)[strKey::Json::GameObject::mComponents];
			if (jValCom.isArray())
			{
				for (Json::ValueConstIterator iter = jValCom.begin(); iter != jValCom.end(); ++iter)
				{
					if (iter->isNull())
					{
						continue;
					}

					if (false == iter->isMember(strKey::Json::Entity::mStrKey))
					{
						continue;
					}

					IComponent* pCom = ComMgr::GetNewCom((*iter)[strKey::Json::Entity::mStrKey].asString());
					if (pCom)
					{
						AddComponent(pCom);
					}
				}
			}
		}

		if (_pJson->isMember(strKey::Json::GameObject::mChilds))
		{
			const Json::Value& jValChilds = (*_pJson)[strKey::Json::GameObject::mChilds];
			if (jValChilds.isArray())
			{
				for (Json::ValueConstIterator iter = jValChilds.begin(); iter != jValChilds.end(); ++iter)
				{
					if (iter->isNull())
					{
						continue;
					}

					if (false == iter->isMember(strKey::Json::Entity::mStrKey))
					{
						continue;
					}

					GameObject* child = new GameObject;
					const Json::Value& childJson = *iter;
					Result = child->LoadJson(&childJson);
					if (eResultFail(Result))
					{
						ERROR_MESSAGE_W(L"Child 오브젝트 로드 실패.");
						SAFE_DELETE(child);
						return Result;
					}

					AddChild(child);
				}
			}
		}

		return eResult::Success;
	}
	
	void GameObject::Init()
	{
		for (size_t i = 0; i < mComponents.size(); ++i)
		{
			if (nullptr == mComponents[i])
				continue;
			mComponents[i]->Init();
		}

		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (nullptr == mChilds[i])
				continue;
			mChilds[i]->Init();
		}
	}

	void GameObject::Update()
	{
		for (size_t i = 0; i < mComponents.size(); ++i)
		{
			if (nullptr == mComponents[i])
				continue;
			mComponents[i]->Update();
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
		for (size_t i = 0; i < mComponents.size(); ++i)
		{
			if (nullptr == mComponents[i])
				continue;
			mComponents[i]->FixedUpdate();
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
		for (size_t i = 0; i < mComponents.size(); ++i)
		{
			if (nullptr == mComponents[i])
				continue;
			mComponents[i]->Render();
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
			mComponents.push_back(_pCom);
			mScripts.push_back(static_cast<IScript*>(_pCom));
		}
		else
		{
			if (nullptr != mComponents[(int)ComType])
			{
				SAFE_DELETE(_pCom);
				ERROR_MESSAGE_W(L"이미 중복된 타입의 컴포넌트가 들어가 있습니다.");
				MH_ASSERT(nullptr);
				return nullptr;
			}

			mComponents[(int)ComType] = _pCom;
		}


		_pCom->SetOwner(this);
		return _pCom;
	}
}
