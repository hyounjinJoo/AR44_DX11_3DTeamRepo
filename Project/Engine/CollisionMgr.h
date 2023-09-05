#pragma once
#include "EnumFlags.h"
#include "ICollider2D.h"
#include "defines.h"


namespace mh
{
	union union_ColliderID
	{
		struct
		{
			UINT32 Left;
			UINT32 Right;
		};
		UINT64 Id;
	};

	//	class Scene;
	class CollisionMgr
	{
		friend class Application;

	public:
		static void Update();
		static void SetCollisionGroup(define::eLayerType _layerType_1, define::eLayerType _layerType_2);
		static std::bitset<define::LAYER_TYPE_COUNT> GetCollisionGroup(define::eLayerType _layerType);

		static void ApplyForceInLayerAtLocation(define::eLayerType _layerType, const float3& _location, const float3& _volume, const float3& _force);
		static void ApplyForceInLayerFromDot(define::eLayerType _layerType, const float3& _location, const float3& _force);



		//static void Update();
		//static void FixedUpdate();
		//static void Render();
		//static void CollisionLayerCheck(define::eLayerType _left, define::eLayerType _right, bool _enable = true);
		//static void LayerCollision(class IScene* _scene, define::eLayerType _left, define::eLayerType _right);
		//static void ColliderCollision(ICollider2D* _left, ICollider2D* _right);
		//static bool Intersect(ICollider2D* _left, ICollider2D* _right);

	private:
		static void Init();
		static void Release();

	private:
		static std::array<std::bitset<define::LAYER_TYPE_COUNT>, define::LAYER_TYPE_COUNT> mArrColGroup;

		static std::bitset<(UINT)define::eLayerType::END> mLayerCollisionMatrix[(UINT)define::eLayerType::END];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
