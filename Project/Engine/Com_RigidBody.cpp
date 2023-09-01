#include "PCH_Engine.h"
#include "Com_RigidBody.h"
#include "Application.h"

#include "CollisionMgr.h"
#include "Com_Transform.h"

#include "GameObject.h"

#include "Physics.h"

#include "EnumFlags.h"

namespace mh
{
	Com_RigidBody::Com_RigidBody()
		: IComponent(define::eComponentType::RigidBody)
		, mMaxVelocity(10.f)
		, mActor(nullptr)
		, mShape(nullptr)
		, mMaterial(nullptr)
		, mbAppliedPhysics(false)
		, mbAppliedGravity(false)
		, mbIsActorInScene(false)
	{
	}
	Com_RigidBody::~Com_RigidBody()
	{
		SAFE_DELETE(mPhysicsInfo.pGeometries);
	}
	void Com_RigidBody::Init()
	{
		if (true == mbAppliedPhysics &&
			false == mbIsActorInScene &&
			nullptr != mActor)
			Application::GetPhysics()->AddActor(GetOwner());

	}
	void Com_RigidBody::FixedUpdate()
	{
		if (true == mbAppliedGravity && false == mbAppliedPhysics)
		{
			AddGravity();
		}

		if (true == mbAppliedPhysics && define::eActorType::Static == mPhysicsInfo.eActorType)
			return;

		else
			GetOwner()->GetComponent<Com_Transform>()->Move(mVelocity);
	}
	void Com_RigidBody::Destroy()
	{
		if (true == mbAppliedPhysics)
		{
			physx::PxRigidActor* pActor = mActor->is<physx::PxRigidActor>();
			pActor->userData = nullptr;
		}
	}
	IComponent* Com_RigidBody::Clone(GameObject* _pGameObject)
	{
		Com_RigidBody* rigidBody = _pGameObject->AddComponent<Com_RigidBody>();

		if (true == mbAppliedPhysics)
		{
			define::tPhysicsInfo info = {};
			info.size = GetGeometrySize();
			info.eActorType = mPhysicsInfo.eActorType;
			info.eGeomType = mPhysicsInfo.eGeomType;
			info.massProperties = mPhysicsInfo.massProperties;

			rigidBody->SetPhysical(info);
			_pGameObject->GetComponent<Com_Transform>()->SetPosition(GetOwner()->GetComponent<Com_Transform>()->GetPosition());
			_pGameObject->GetComponent<Com_Transform>()->SetRotation(GetOwner()->GetComponent<Com_Transform>()->GetRotation());
			_pGameObject->GetComponent<Com_Transform>()->SetScale(GetOwner()->GetComponent<Com_Transform>()->GetScale());
		}

		if (define::eActorType::Static != mPhysicsInfo.eActorType)
		{
			rigidBody->SetVelocity(mVelocity);
			rigidBody->SetMaxVelocity(mMaxVelocity);
		}

		return rigidBody;
	}
	void Com_RigidBody::SetPhysical(const define::tPhysicsInfo& _physicsInfo)
	{
		mPhysicsInfo = _physicsInfo;

		CreateMaterial();
		CreateGeometry();
		CreateActor();
		CreateShape();

		mbAppliedPhysics = true;
		InitializeActor();
	}
	bool Com_RigidBody::IsAppliedPhysics()
	{
		return mbAppliedPhysics;
	}
	void Com_RigidBody::AddActorToScene()
	{
		AssertEx(mbAppliedPhysics, L"RigidBody::AddActorToScene() - 물리가 들어가지 않은 오브젝트에 대한 AddActorToScene 호출");
		AssertEx(mActor, L"RigidBody::AddActorToScene() - mpActor가 생성되지 않음");

		Application::GetPhysics()->AddActor(GetOwner());
		mbIsActorInScene = true;
	}
	physx::PxTransform Com_RigidBody::GetPhysicsTransform()
	{
		AssertEx(mbAppliedPhysics, L"RigidBody::GetPhysicsTransform() - 물리가 들어가지 않은 오브젝트에 대한 GetPhysicsTransform 호출");
		return GetActor<physx::PxRigidActor>()->getGlobalPose();
	}
	void Com_RigidBody::SetPhysicsTransform(physx::PxTransform _transform)
	{
		//_transform.p.z = -_transform.p.z;
		AssertEx(mbAppliedPhysics, L"RigidBody::SetPhysicsTransform() - 물리가 들어가지 않은 오브젝트에 대한 SetPhysicsTransform 호출");
		GetActor<physx::PxRigidActor>()->setGlobalPose(_transform);
	}
	void Com_RigidBody::SetVelocity(const float3& _velocity)
	{
		if (true == mbAppliedPhysics)
		{
			AssertEx(define::eActorType::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::SetVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
			GetDynamicActor()->setLinearVelocity(_velocity);
		}
		else
		{
			mVelocity = _velocity;
		}

		CheckMaxVelocity();
	}
	void Com_RigidBody::SetVelocity(define::eAxis3D _eAxis, float _velocity)
	{
		if (true == mbAppliedPhysics)
		{
			AssertEx(define::eActorType::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::SetVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
			float3 velocity = GetDynamicActor()->getLinearVelocity();
			switch (_eAxis)
			{
			case define::eAxis3D::X:
				velocity.x = _velocity;
				break;
			case define::eAxis3D::Y:
				velocity.y = _velocity;
				break;
			case define::eAxis3D::Z:
				velocity.z = _velocity;
				break;
			}
			GetDynamicActor()->setLinearVelocity(velocity);

			mVelocity = velocity;
		}
		else
		{
			switch (_eAxis)
			{
			case define::eAxis3D::X:
				mVelocity.x = _velocity;
				break;
			case define::eAxis3D::Y:
				mVelocity.y = _velocity;
				break;
			case define::eAxis3D::Z:
				mVelocity.z = _velocity;
				break;
			}
		}

		CheckMaxVelocity();
	}
	void Com_RigidBody::AddVelocity(const float3& _velocity)
	{
		if (true == mbAppliedPhysics)
		{
			AssertEx(define::eActorType::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::AddVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
			float3 velocity = GetDynamicActor()->getLinearVelocity();
			velocity += _velocity;
			GetDynamicActor()->setLinearVelocity(velocity);
		}
		else
		{
			mVelocity = _velocity;
		}

		CheckMaxVelocity();
	}
	void Com_RigidBody::AddVelocity(define::eAxis3D _eAxis, float _velocity)
	{
		if (true == mbAppliedPhysics)
		{
			AssertEx(define::eActorType::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::AddVelocity() - Dynamic Actor가 아닌 물체에 대한 SetVelocity() 호출 시도");
			float3 velocity = GetDynamicActor()->getLinearVelocity();
			switch (_eAxis)
			{
			case define::eAxis3D::X:
				velocity.x += _velocity;
				break;
			case define::eAxis3D::Y:
				velocity.y += _velocity;
				break;
			case define::eAxis3D::Z:
				velocity.z += _velocity;
				break;
			}
			GetDynamicActor()->setLinearVelocity(velocity);
		}
		else
		{
			switch (_eAxis)
			{
			case define::eAxis3D::X:
				mVelocity.x += _velocity;
				break;
			case define::eAxis3D::Y:
				mVelocity.y += _velocity;
				break;
			case define::eAxis3D::Z:
				mVelocity.z += _velocity;
				break;
			}
		}
		CheckMaxVelocity();
	}
	void Com_RigidBody::AddGravity()
	{
		mVelocity += GLOBAL_GRAVITY;
	}
	void Com_RigidBody::SetMaxVelocity(float _maxVelocity)
	{
		if (true == mbAppliedPhysics)
		{
			AssertEx(define::eActorType::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::SetMaxVelocity() - Dynamic Actor가 아닌 물체에 대한 SetMaxVelocity() 호출 시도");
			GetDynamicActor()->setMaxLinearVelocity(_maxVelocity);
		}

		mMaxVelocity = _maxVelocity;
	}
	void Com_RigidBody::CheckMaxVelocity()
	{
		mVelocity.x = mVelocity.x < 0.f ? max(mVelocity.x, -mMaxVelocity) : min(mVelocity.x, mMaxVelocity);
		mVelocity.y = mVelocity.y < 0.f ? max(mVelocity.y, -mMaxVelocity) : min(mVelocity.y, mMaxVelocity);
		mVelocity.z = mVelocity.z < 0.f ? max(mVelocity.z, -mMaxVelocity) : min(mVelocity.z, mMaxVelocity);
	}
	void Com_RigidBody::ApplyGravity()
	{
		if (true == mbAppliedPhysics)
		{
			AssertEx(define::eActorType::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::ApplyGravity() - Dynamic Actor가 아닌 물체에 대한 ApplyGravity() 호출 시도");
			GetDynamicActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
		}

		mbAppliedGravity = true;
	}
	void Com_RigidBody::RemoveGravity()
	{
		if (true == mbAppliedPhysics)
		{
			AssertEx(define::eActorType::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::RemoveGravity() - Dynamic Actor가 아닌 물체에 대한 RemoveGravity() 호출 시도");
			GetDynamicActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
		}

		mbAppliedGravity = false;
	}
	void Com_RigidBody::SetFreezeRotation(FreezeRotationFlag flag, bool enable)
	{
		physx::PxActor* actor = mShape->getActor();
		assert(actor);

		physx::PxRigidDynamic* rigidActor = actor->is<physx::PxRigidDynamic>();
		if (rigidActor == nullptr)
			return;

		EnumFlags<FreezeRotationFlag, uint16_t> enumFlag{ flag };

		if (enable)
		{
			mFreezeRotationFlag |= enumFlag;
			if (enumFlag & FreezeRotationFlag::ROTATION_X)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
			if (enumFlag & FreezeRotationFlag::ROTATION_Y)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
			if (enumFlag & FreezeRotationFlag::ROTATION_Z)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
		}
		else
		{
			mFreezeRotationFlag &= ~enumFlag;
			if (enumFlag & FreezeRotationFlag::ROTATION_X)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
			if (enumFlag & FreezeRotationFlag::ROTATION_Y)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
			if (enumFlag & FreezeRotationFlag::ROTATION_Z)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);
		}

		rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, true);
		rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
	}

	void Com_RigidBody::SetLinearDamping(float _damping)
	{
		AssertEx(define::eActorType::Dynamic == mPhysicsInfo.eActorType, L"RigidBody::SetLinearDamping() - Dynamic Actor가 아닌 물체에 대한 SetLinearDamping() 호출 시도");
		GetActor<physx::PxRigidDynamic>()->setLinearDamping(_damping);
	}
	void Com_RigidBody::SetSimulationShapeFlag(bool _bFlag)
	{
		AssertEx(mbAppliedPhysics, L"RigidBody::SetSimulationShapeFlag() - 물리가 들어가지 않은 오브젝트에 대한 SetSimulationShapeFlag 호출");
		mShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, _bFlag);
	}
	void Com_RigidBody::SetTriggerShapeFlag(bool _bFlag)
	{
		AssertEx(mbAppliedPhysics, L"RigidBody::SetTriggerShapeFlag() - 물리가 들어가지 않은 오브젝트에 대한 SetTriggerShapeFlag 호출");
		mShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, _bFlag);
	}
	void Com_RigidBody::SetActorInSceneFlag(bool _bFlag)
	{
		mbIsActorInScene = _bFlag;
	}
	void Com_RigidBody::AddForce(const float3& _force)
	{
		AssertEx(mbAppliedPhysics, L"RigidBody::AddForce() - 물리가 들어가지 않은 오브젝트에 대한 AddForce 호출");
		AssertEx(define::eActorType::Static != mPhysicsInfo.eActorType, L"RigidBody::AddForce() - Static 객체에 대해 힘 적용");

		PxRigidBodyExt::addForceAtPos(
			*GetDynamicActor(),
			_force,
			GetOwner()->GetComponent<Com_Transform>()->GetPosition(),
			physx::PxForceMode::eIMPULSE
		);
	}
	void Com_RigidBody::CreateBoxGeometry()
	{
		mPhysicsInfo.pGeometries = new Geometries(mPhysicsInfo.eGeomType, mPhysicsInfo.size);
	}
	void Com_RigidBody::CreateCapsuleGeometry()
	{
		mPhysicsInfo.pGeometries = new Geometries(mPhysicsInfo.eGeomType, mPhysicsInfo.size.x, mPhysicsInfo.size.y);
	}
	void Com_RigidBody::CreatePlaneGeometry()
	{
		mPhysicsInfo.pGeometries = new Geometries(mPhysicsInfo.eGeomType);
	}
	void Com_RigidBody::CreateSphereGeometry()
	{
		mPhysicsInfo.pGeometries = new Geometries(mPhysicsInfo.eGeomType, mPhysicsInfo.size.x);
	}
	void Com_RigidBody::CreateGeometry()
	{
		mPhysicsInfo.size /= 2.f;
		switch (mPhysicsInfo.eGeomType)
		{
		case define::eGeometryType::Box:
			CreateBoxGeometry();
			break;

		case define::eGeometryType::Capsule:
			CreateCapsuleGeometry();
			break;

		case define::eGeometryType::Sphere:
			CreateSphereGeometry();
			break;

		case define::eGeometryType::Plane:
			CreatePlaneGeometry();
			break;
		}

		AssertEx(mPhysicsInfo.pGeometries, L"RigidBody::CreateGeometry() - Geometry 생성 실패");
	}
	void Com_RigidBody::CreateShape()
	{
		switch (mPhysicsInfo.eGeomType)
		{
		case define::eGeometryType::Box:
			mShape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mPhysicsInfo.pGeometries->boxGeom, *mMaterial);
			break;
		case define::eGeometryType::Capsule:
			mShape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mPhysicsInfo.pGeometries->capsuleGeom, *mMaterial);
			break;
		case define::eGeometryType::Sphere:
			mShape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mPhysicsInfo.pGeometries->sphereGeom, *mMaterial);
			break;
		case define::eGeometryType::Plane:
			mShape = physx::PxRigidActorExt::createExclusiveShape(*mActor->is<physx::PxRigidActor>(), mPhysicsInfo.pGeometries->planeGeom, *mMaterial);
			break;
		}

		AssertEx(mShape, L"RigidBody::CreateShape() - Shape 생성 실패");
	}
	void Com_RigidBody::CreateActor()
	{
		switch (mPhysicsInfo.eActorType)
		{
		case define::eActorType::Dynamic:
			mActor = PHYSICS->createRigidDynamic(physx::PxTransform(GetOwner()->GetComponent<Com_Transform>()->GetPosition()));
			break;

		case define::eActorType::Static:
			mActor = PHYSICS->createRigidStatic(physx::PxTransform(GetOwner()->GetComponent<Com_Transform>()->GetPosition()));
			break;

		case define::eActorType::Kinematic:
			mActor = PHYSICS->createRigidDynamic(physx::PxTransform(GetOwner()->GetComponent<Com_Transform>()->GetPosition()));
			mActor->is<physx::PxRigidDynamic>()->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
			break;
		}

		AssertEx(mActor, L"RigidBody::CreateActor() - Actor 생성 실패");
	}
	void Com_RigidBody::CreateMaterial()
	{
		mMaterial = PHYSICS->createMaterial(mPhysicsInfo.massProperties.staticFriction,
											mPhysicsInfo.massProperties.dynamicFriction,
											mPhysicsInfo.massProperties.restitution);
	}
	void Com_RigidBody::InitializeActor()
	{
		physx::PxRigidActor* pActor = mActor->is<physx::PxRigidActor>();
		pActor->userData = GetOwner();

		mPhysicsInfo.filterData.word0 = 1 << static_cast<int>(GetOwner()->GetLayerType());

		std::bitset<LAYER_TYPE_COUNT> collisionGroup = CollisionMgr::GetCollisionGroup(GetOwner()->GetLayerType());

		for (int i = 0; i < LAYER_TYPE_COUNT; ++i)
		{
			if (true == collisionGroup[i])
				mPhysicsInfo.filterData.word1 |= 1 << i;
		}

		mShape->setSimulationFilterData(mPhysicsInfo.filterData);
		physx::PxVec3 myPos = GetOwner()->GetComponent<Com_Transform>()->GetPosition();
		pActor->setGlobalPose(physx::PxTransform(myPos));

		switch (mPhysicsInfo.eActorType)
		{
		case define::eActorType::Static:
			break;
		case define::eActorType::Dynamic:
			ApplyGravity();
			break;
		case define::eActorType::Kinematic:
			SetSimulationShapeFlag(false);
			SetTriggerShapeFlag(true);
			break;
		}
	}
}
