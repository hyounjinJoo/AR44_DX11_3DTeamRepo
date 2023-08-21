
#include "PCH_Engine.h"

#include "Layer.h"
#include "RenderMgr.h"
#include "Com_Transform.h"


namespace mh
{
	// z값 정렬 작성중
	//static bool CompareGameObjectByZAxis(GameObject* a, GameObject* b)
	//{
	//	Transform* aTr = a->GetTransform();
	//	Transform* bTr = b->GetTransform();

	//	if (aTr->GetRelativePos().z <= bTr->GetRelativePos().z)
	//	{
	//		return true;
	//	}

	//	return false;
	//}

	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
		auto iter = std::remove_if(mGameObjects.begin(), mGameObjects.end(),
			[](GameObject* _obj)->bool
			{
				return !(_obj->IsMaster());
			});
		mGameObjects.erase(iter, mGameObjects.end());
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			delete mGameObjects[i];
		}
	}

	void Layer::Init()
	{
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			if (mGameObjects[i]->IsMaster())
				mGameObjects[i]->Init();
		}
	}

	void Layer::Update()
	{
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			if (mGameObjects[i]->IsMaster() && GameObject::eState::Active == mGameObjects[i]->GetState())
				mGameObjects[i]->Update();
		}
	}

	void Layer::FixedUpdate()
	{
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			if (mGameObjects[i]->IsMaster() && GameObject::eState::Active == mGameObjects[i]->GetState())
				mGameObjects[i]->FixedUpdate();
		}

		// sort z axis
		//std::vector<GameObject*> mGameObjects;
		//std::sort(mGameObjects.begin(), mGameObjects.end(), CompareGameObjectByZAxis);
	}

	void Layer::Render()
	{
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			if (GameObject::eState::Active == mGameObjects[i]->GetState())
				mGameObjects[i]->Render();
		}
	}

	void Layer::Destroy()
	{
		std::set<GameObject*> deleteObjects;
		// 삭제할 오브젝트들을 전부 찾아온다.
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj->GetState() == GameObject::eState::Dead)
			{
				deleteObjects.insert(gameObj);
			}
		}

		// 지워야할 오브젝트들 게임 오브젝트 모음안에서 삭제
		for (GameObjectIter iter = mGameObjects.begin()
			; iter != mGameObjects.end()
			; )
		{
			std::set<GameObject*>::iterator deleteIter
				= deleteObjects.find(*iter);

			if (deleteIter != deleteObjects.end())
			{
				iter = mGameObjects.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		// 삭제할 오브젝트들을 실제 램(메모리)에서 삭제
		for (GameObject* gameObj : deleteObjects)
		{
			delete gameObj;
			gameObj = nullptr;
		}
	}

	void Layer::AddGameObject(GameObject* gameObject, bool _bNeedInit)
	{
		if (gameObject == nullptr)
			return;

		mGameObjects.push_back(gameObject);

		if (_bNeedInit)
			gameObject->Init();
	}
	std::vector<GameObject*> Layer::GetDontDestroyGameObjects()
	{
		std::vector<GameObject*> donts;

		//mGameObjects에서 삭제 안할 게임오브젝트들을 제거하고 
		//그걸 donts 벡터에 추가
		//나머지는 냄겨둠
		mGameObjects.erase
		(
			std::remove_if(mGameObjects.begin(), mGameObjects.end(), 
				[&](GameObject* _obj)
				{
					bool result = false;
					if (_obj && _obj->IsDontDestroy())
					{
						donts.push_back(_obj);
						result = true;
					}

					return result;
				})
			, mGameObjects.end()
		);

		return donts;
	}
}
