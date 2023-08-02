#include "EnginePCH.h"

#include "Com_AudioSource.h"
#include "AudioClip.h"
#include "Com_Transform.h"
#include "GameObject.h"

#include "json-cpp/json.h"
#include "ResMgr.h"

namespace mh
{
	Com_AudioSource::Com_AudioSource()
		: IComponent(define::eComponentType::AudioSource)
		, mAudioClip(nullptr)
	{

	}

	eResult Com_AudioSource::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail;
		}
		eResult result = IComponent::SaveJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		Json::Value& jVal = *_pJson;

		if (mAudioClip)
		{
			jVal[JSONKEY(mAudioClip)] = mAudioClip->GetKey();
		}

		return eResult::Success;
	}

	eResult Com_AudioSource::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail;
		}
		eResult result = IComponent::LoadJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		const Json::Value& jVal = *_pJson;

		if (jVal.isMember(JSONKEY(mAudioClip)))
		{
			std::string strKey = jVal[JSONKEY(mAudilClip)].asString();
			mAudioClip = ResMgr::Load<AudioClip>(strKey);
		}

		return eResult::Success;
	}

	Com_AudioSource::~Com_AudioSource()
	{

	}

	void Com_AudioSource::Init()
	{
	}

	void Com_AudioSource::Update()
	{
	}

	void Com_AudioSource::FixedUpdate()
	{
		Com_Transform& tr = GetOwner()->GetTransform();
		math::Vector3 pos = tr.GetPosition();
		math::Vector3 foward = tr.Foward();

		mAudioClip->Set3DAttributes(pos, foward);
	}

	void Com_AudioSource::Render()
	{
	}

	void Com_AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void Com_AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void Com_AudioSource::SetLoop(bool _loop)
	{
		mAudioClip->SetLoop(_loop);
	}
}
