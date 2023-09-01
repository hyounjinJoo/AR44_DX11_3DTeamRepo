#include "PCH_Engine.h"
#include "Physics.h"
#include "defines.h"
#include "GameObject.h"
#include "ContactCallback.h"
#include "Com_RigidBody.h"
#include "TimeMgr.h"

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
		mScene->simulate(TimeMgr::DeltaTime());
		mScene->fetchResults(true);


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
		AssertEx(_gameObject, L"Physics::RemoveActor() - GameObject is nullptr");
		AssertEx(_gameObject->GetComponent<Com_RigidBody>(), L"Physics::RemoveActor() - RigidBody is nullptr");
		AssertEx(_gameObject->GetComponent<Com_RigidBody>()->IsAppliedPhysics(), L"Physics::RemoveActor() - Is not applied physics");
		mScene->removeActor(*_gameObject->GetComponent<Com_RigidBody>()->GetActor());
	}
	PxFilterFlags Physics::PlayerFilterShader(PxFilterObjectAttributes _attributes0, PxFilterData _filterData0, PxFilterObjectAttributes _attributes1, PxFilterData _filterData1, PxPairFlags& _pairFlags, const void* _constantBlock, PxU32 _constantBlockSize)
	{
		// 트리거와 트리거 또는 트리거와 충돌하는 물체를 구분하여 처리합니다.
		if (PxFilterObjectIsTrigger(_attributes0) || PxFilterObjectIsTrigger(_attributes1))
		{
			if ((_filterData1.word1 & _filterData0.word0) || (_filterData0.word1 & _filterData1.word0))
			{
				// 트리거와 충돌하는 물체 모두 OnTrigger 이벤트를 처리하도록 합니다.
				_pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
				return PxFilterFlag::eDEFAULT;
			}
		}

		// 충돌하는 물체의 경우 충돌 플래그만 생성합니다.
		_pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		// 두 필터가 서로 충돌플래그가 세워져 있을 경우
		if ((_filterData0.word0 & _filterData1.word1) || (_filterData1.word0 & _filterData0.word1))
		{
			_pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
			return PxFilterFlag::eDEFAULT;
		}

		return PxFilterFlag::eKILL;
	}
}
