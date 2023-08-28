
#include "PCH_Engine.h"

#include "Layer.h"
#include "RenderMgr.h"
#include "Com_Transform.h"


namespace mh
{

	Layer::Layer()
		: mLayerType()
		, mGameObjects{}
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
		std::vector<GameObject*> masterObjects;
		auto iter = std::remove_if(mGameObjects.begin(), mGameObjects.end(),
			[&](GameObject* _obj)->bool
			{
				bool bRet = false;
				if (GameObject::eState::Dead == _obj->GetState())
				{
					if (_obj->IsMaster())
						masterObjects.push_back(_obj);
					bRet = true;
				}
				return bRet;
			}
		);
		mGameObjects.erase(iter, mGameObjects.end());

		for (size_t i = 0; i < masterObjects.size(); ++i)
		{
			delete masterObjects[i];
		}
	}

	void Layer::AddGameObject(GameObject* gameObject, bool _bNeedInit)
	{
		if (gameObject == nullptr)
			return;

		mGameObjects.push_back(gameObject);
		gameObject->SetLayerType(mLayerType);

		if (_bNeedInit)
			gameObject->Init();
	}

	void Layer::RemoveGameObject(const GameObject* gameObject)
	{
		if (gameObject)
		{
			auto iter = mGameObjects.begin();
			const auto& iterEnd = mGameObjects.end();
			for (iter; iter != iterEnd; ++iter)
			{
				if (gameObject == (*iter))
				{
					mGameObjects.erase(iter);
					break;
				}
			}
		}
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
