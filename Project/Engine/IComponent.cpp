#include "PCH_Engine.h"

#include "IComponent.h"
#include "GameObject.h"
#include "json-cpp\jsonSaveLoad.h"

namespace mh
{
	using namespace mh::define;

	IComponent::IComponent(define::eComponentType _type)
		: mType(_type)
		, mComTypeID()
		, mOwner(nullptr)
	{
	}

	IComponent::IComponent(const IComponent& _other)
		: Entity(_other)
		, mType(_other.mType)
		, mOwner(nullptr)
	{
	}

	IComponent::~IComponent()
	{
	}
	eResult IComponent::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = Entity::SaveJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}

		Json::MH::SaveValue(_pJson, JSON_KEY_PAIR(mType));

		return eResult::Success;
	}
	eResult IComponent::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = Entity::LoadJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}

		//MH_SAVE_VALUE(_pJson, mType);

		return eResult::Success;
	}
	bool IComponent::IsPhysicsObject()
	{
		return GetOwner()->IsPhysicsObject();
	}
}
