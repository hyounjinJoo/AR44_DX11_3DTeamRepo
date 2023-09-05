#include "PCH_Engine.h"
#include "CollisionMgr.h"
#include "defines.h"
#include "IScene.h"
#include "SceneMgr.h"

#include "AtExit.h"



namespace mh
{
	std::array<std::bitset<define::LAYER_TYPE_COUNT>, define::LAYER_TYPE_COUNT> CollisionMgr::mArrColGroup{};

	std::bitset<(UINT)define::eLayerType::END> CollisionMgr::mLayerCollisionMatrix[(UINT)define::eLayerType::END]{};
	std::map<UINT64, bool> CollisionMgr::mCollisionMap{};

	void CollisionMgr::Update()
	{

	}

	void CollisionMgr::SetCollisionGroup(define::eLayerType _layerType_1, define::eLayerType _layerType_2)
	{
		mArrColGroup[static_cast<int>(_layerType_1)][static_cast<int>(_layerType_2)] = true;


		IScene* pActiveScene = SceneMgr::GetActiveScene();

		std::vector<GameObject*> gameObjects_1 = pActiveScene->GetGameObjects(_layerType_1);
		for (GameObject* pGameObject : gameObjects_1)
		{
			pGameObject->GetComponent<Com_RigidBody>()->SetOtherLayerInFilterData(_layerType_2);
		}
	}

	std::bitset<define::LAYER_TYPE_COUNT> CollisionMgr::GetCollisionGroup(define::eLayerType _layerType)
	{
		return mArrColGroup[static_cast<int>(_layerType)];
	}

	void CollisionMgr::ApplyForceInLayerAtLocation(define::eLayerType _layerType, const float3& _location, const float3& _volume, const float3& _force)
	{
	}

	void CollisionMgr::ApplyForceInLayerFromDot(define::eLayerType _layerType, const float3& _location, const float3& _force)
	{
	}
	void CollisionMgr::Init()
	{
		AtExit::AddFunc(Release);
	}
	void CollisionMgr::Release()
	{
	}
}
