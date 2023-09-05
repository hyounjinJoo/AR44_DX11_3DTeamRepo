#pragma once
#include <PhysX/PxPhysicsAPI.h>
#include "SimpleMath.h"

using namespace physx;
namespace mh
{
	class GameObject;
	class Physics
	{
	public:
		Physics();
		~Physics();

	public:
		void Initialize();
		void Update();
		void Render();

	public:
		void AddActor(GameObject* _gameObject);
		void RemoveActor(GameObject* _gameObject);

		const float3& GetGravity() { return mGravity; }
		PxPhysics* GetPhysics() { return mPhysics; }

		static PxFilterFlags PlayerFilterShader(
			PxFilterObjectAttributes _attributes0, PxFilterData _filterData0,
			PxFilterObjectAttributes _attributes1, PxFilterData _filterData1,
			PxPairFlags& _pairFlags, const void* _constantBlock, PxU32 _constantBlockSize);

	private:
		PxDefaultAllocator		mAllocCallback;
		PxDefaultErrorCallback  mErrorCallback;
		PxFoundation* mFoundation;
		PxPhysics* mPhysics;

		PxPvdTransport* mTransfort;
		PxPvd* mPvd;
		PxPvdSceneClient* mSceneClient;
		PxScene* mScene;
		PxControllerManager* mControllerMgr;

		PxDefaultCpuDispatcher* mCpuDispatcher;
		class ContactCallback* mCallback;

		float3 mGravity;
	};

}

