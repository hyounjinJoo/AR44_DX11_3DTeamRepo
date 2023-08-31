#include "PCH_Engine.h"
#include "ICollider3D.h"
#include "Com_Transform.h"
#include "ResMgr.h"
#include "json-cpp/json.h"
#include "GameObject.h"

namespace mh
{
	using namespace mh::define;

	ICollider3D::ICollider3D()
		: ICollider(define::eColliderType::None)
	{
		MH_ASSERT(TRUE);
	}
	
	ICollider3D::ICollider3D(const ICollider3D& _collider)
		: ICollider(_collider.mType)
	{
		MH_ASSERT(TRUE);
		mMesh = ResMgr::Find<Mesh>(define::strKey::Default::mesh::CubeMesh);
		mMaterial = ResMgr::Find<Material>(define::strKey::Default::material::DebugMaterial);
	}

	ICollider3D::~ICollider3D()
	{
	}

	eResult ICollider3D::SaveJson(Json::Value* _pJVal)
	{
		MH_ASSERT(TRUE);

		//if (nullptr == _pJVal)
		//{
		//	return eResult::Fail_Nullptr;
		//}

		//eResult result = ICollider::SaveJson(_pJVal);
		//if (eResultFail(result))
		//{
		//	return result;
		//}

		//Json::Value& jVal = *_pJVal;

		//Json::MH::SaveValue(_pJVal, JSONVAL(mType));

		return eResult::Success;
	}

	eResult ICollider3D::LoadJson(const Json::Value* _pJVal)
	{
		MH_ASSERT(TRUE);

		//if (nullptr == _pJVal)
		//{
		//	return eResult::Fail_Nullptr;
		//}

		//eResult result = ICollider::LoadJson(_pJVal);
		//if (eResultFail(result))
		//{
		//	return result;
		//}

		//const Json::Value& jVal = (*_pJVal);

		//Json::MH::LoadValue(_pJVal, JSONVAL(mType));
		
		return eResult::Success;
	}

	void ICollider3D::Init()
	{
		AssertEx(IsPhysicsObject(), L"Collider::Initialize() - 충돌을 사용하기 위해서는 RigidBody->SetPhysical()가 선행되어야 함.");
	}

	void ICollider3D::Update()
	{
	}

	void ICollider3D::FixedUpdate()
	{
		
	}

	void ICollider3D::OnCollisionEnter(ICollider3D* _otherCollider)
	{
		//mCollisionCount++;
	}

	void ICollider3D::OnCollisionStay(ICollider3D* _otherCollider)
	{
	}

	void ICollider3D::OnCollisionExit(ICollider3D* _otherCollider)
	{
		mCollisionCount--;

		if (0 > mCollisionCount)
			mCollisionCount = 0;
	}

	void ICollider3D::OnTriggerEnter(ICollider3D* _otherCollider)
	{
		mCollisionCount++;
	}

	void ICollider3D::OnTriggerStay(ICollider3D* _otherCollider)
	{
	}

	void ICollider3D::OnTriggerExit(ICollider3D* _otherCollider)
	{
		mCollisionCount--;

		if (0 > mCollisionCount)
			mCollisionCount = 0;
	}
}
