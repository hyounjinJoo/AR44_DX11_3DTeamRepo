#include "EnginePCH.h"

#include "IComponent.h"

#include "json-cpp\jsonSaveLoad.h"

namespace mh
{
	IComponent::IComponent(define::eComponentType _type)
		: mType(_type)
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

		MH_SAVE_VALUE(_pJson, mType);

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
}
