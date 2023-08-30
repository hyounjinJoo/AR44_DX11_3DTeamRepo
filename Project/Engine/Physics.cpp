#include "PCH_Engine.h"
#include "Physics.h"
#include "defines.h"
#include "GameObject.h"
#include "ContactCallback.h"
#include "Com_RigidBody.h"

namespace mh
{
	Physics::Physics()
		:mFoundation(nullptr)
		, mPhysics(nullptr)
		, mTransfort(nullptr)
		, mPvd(nullptr)
		, mSceneClient(nullptr)
		, mScene(nullptr)
		, mControllerMgr(nullptr)
		, mCpuDispatcher(nullptr)
		, mCallback(nullptr)
		, mGravity(0.f, -9.81f, 0.f)
	{
	}
	Physics::~Physics()
	{
		mCpuDispatcher->release();

		SAFE_DELETE(mCallback);
		mScene->release();
		mPhysics->release();
		mPvd->release();
		mTransfort->release();
		mFoundation->release();
	}
	void Physics::Initialize()
	{
		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocCallback, mErrorCallback);

		mTransfort = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		mPvd = PxCreatePvd(*mFoundation);
		mPvd->connect(*mTransfort, PxPvdInstrumentationFlag::eALL);

		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), false, mPvd);

		mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		mCallback = new ContactCallback;

		PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
		sceneDesc.gravity = mGravity;
		sceneDesc.cpuDispatcher = mCpuDispatcher;
		sceneDesc.filterShader = PlayerFilterShader;
		sceneDesc.simulationEventCallback = mCallback;

		mScene = mPhysics->createScene(sceneDesc);
		mSceneClient = mScene->getScenePvdClient();

		mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		mSceneClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	void Physics::Update()
	{
	}
	void Physics::Render()
	{
	}
	void Physics::AddActor(GameObject* _gameObject)
	{
		AssertEx(_gameObject, L"Physics::AddActor() - GameObject is nullptr");
		AssertEx(_gameObject->GetComponent<Com_RigidBody>(), L"Physics::AddActor() - RigidBody is nullptr");
		AssertEx(_gameObject->GetComponent<Com_RigidBody>()->IsAppliedPhysics(), L"Physics::AddActor() - Is not applied physics");
		mScene->addActor(*_gameObject->GetComponent<Com_RigidBody>()->GetActor());
	}
	void Physics::RemoveActor(GameObject* _gameObject)
	{
	}
	PxFilterFlags Physics::PlayerFilterShader(PxFilterObjectAttributes _attributes0, PxFilterData _filterData0, PxFilterObjectAttributes _attributes1, PxFilterData _filterData1, PxPairFlags& _pairFlags, const void* _constantBlock, PxU32 _constantBlockSize)
	{
		return PxFilterFlags();
	}
}
