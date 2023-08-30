#pragma once
#include "IComponent.h"
#include "SimpleMath.h"
#include "defines.h"

namespace mh
{
	class Com_RigidBody :
		public IComponent
	{
	public:
		Com_RigidBody();
		virtual ~Com_RigidBody();

		virtual void Initialize();
		virtual void FixedUpdate();
		virtual void FinalUpdate();
		virtual void Destroy();

		virtual IComponent* Clone(GameObject* _pGameObject);

		void SetPhysical(const define::tPhysicsInfo& _physicsInfo = define::tPhysicsInfo());
		bool IsAppliedPhysics();

		physx::PxActor* GetActor() { return mActor; }

		template<typename T>
		inline T* GetActor() const
		{
			T* pActor = mpActor->is<T>();
			assert(pActor);
			return pActor;
		}

		void AddActorToScene();

		physx::PxRigidDynamic* GetDynamicActor() { return GetActor<physx::PxRigidDynamic>(); }
		physx::PxRigidStatic* GetStaticActor() { return GetActor<physx::PxRigidStatic>(); }

		physx::PxTransform GetPhysicsTransform();
		void			   SetPhysicsTransform(physx::PxTransform _transform);

		define::eActorType GetActorType() { return mPhysicsInfo.eActorType; }
		define::eGeometryType GetGeometryType() { return mPhysicsInfo.eGeometryType; }
		float3 GetGeometrySize() { return mPhysicsInfo.size * 2.f; }
		const physx::PxFilterData& GetFilterData() { return mPhysicsInfo.filterData; }
		void SetOtherLayerInFilterData(define::eLayerType _eOtherLayer) { mPhysicsInfo.filterData.word1 |= 1 << static_cast<int>(_eOtherLayer); }

		void SetVelocity(const float3& _velocity);
		void SetVelocity(define::eAxis3D _eAxis, float _velocity);
		void AddVelocity(const float3& _velocity);
		void AddVelocity(define::eAxis3D _eAxis, float _velocity);
		void AddGravity();

		void SetMaxVelocity(float _maxVelocity);
		void CheckMaxVelocity();

		void ApplyGravity();
		void RemoveGravity();

		void SetLinearDamping(float _damping);
		void SetSimulationShapeFlag(bool _bFlag);
		void SetTriggerShapeFlag(bool _bFlag);

		void SetActorInSceneFlag(bool _bFlag);
		bool IsActorInScene() { return mbIsActorInScene; }

		void AddForce(const float3& _force);

	private:
		void CreateBoxGeometry();
		void CreateCapsuleGeometry();
		void CreatePlaneGeometry();
		void CreateSphereGeometry();

	private:
		void CreateGeometry();
		void CreateShape();
		void CreateActor();
		void CreateMaterial();
		void InitializeActor();

	private:
		define::tPhysicsInfo mPhysicsInfo;

		physx::PxActor* mActor;
		physx::PxShape* mShape;
		physx::PxMaterial* mMaterial;

		float3 mVelocity;
		float mMaxVelocity;

		bool mbAppliedPhysics;
		bool mbAppliedGravity;
		bool mbIsActorInScene;
	};
}
