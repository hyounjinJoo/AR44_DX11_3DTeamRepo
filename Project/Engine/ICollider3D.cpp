#include "PCH_Engine.h"
#include "ICollider3D.h"
#include "Com_Transform.h"

#include "json-cpp/json.h"

namespace mh
{
	ICollider3D::ICollider3D()
		: ICollider(define::eColliderType::None)
	{
		MH_ASSERT(TRUE);
	}
	
	ICollider3D::ICollider3D(const ICollider3D& _collider)
		: ICollider(_collider.mType)
	{
		MH_ASSERT(TRUE);
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

		//Json::MHSaveValue(_pJVal, JSONVAL(mType));

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

		//Json::MHLoadValue(_pJVal, JSONVAL(mType));
		
		return eResult::Success;
	}

	void ICollider3D::Init()
	{
	}

	void ICollider3D::Update()
	{
	}

	void ICollider3D::FixedUpdate()
	{
	}

	void ICollider3D::Render()
	{
	}
}
