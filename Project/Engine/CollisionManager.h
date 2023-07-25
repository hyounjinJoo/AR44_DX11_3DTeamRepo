#pragma once
#include "EnumFlags.h"
#include "Collider2D.h"

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
	class CollisionManager
	{
	public:
		static void Initialize();
		static void Update();
		static void FixedUpdate();
		static void Render();

		static void CollisionLayerCheck(define::eLayerType _left, define::eLayerType _right, bool _enable = true);
		static void LayerCollision(class Scene* _scene, define::eLayerType _left, define::eLayerType _right);
		static void ColliderCollision(Collider2D* _left, Collider2D* _right);
		static bool Intersect(Collider2D* _left, Collider2D* _right);

	private:
		static std::bitset<(UINT)define::eLayerType::End> mLayerCollisionMatrix[(UINT)define::eLayerType::End];
		static std::map<UINT64, bool> mCollisionMap;
	};
}