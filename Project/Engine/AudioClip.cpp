#include "EnginePCH.h"

#include "AudioClip.h"
#include "Com_Transform.h"

namespace mh
{
	AudioClip::AudioClip()
		: IRes(define::eResourceType::AudioClip)
		, mSound(nullptr)
		, mChannel(nullptr)
		, mMinDistance(1.0f)
		, mMaxDistance(1000.0f)
		, mbLoop(false)
	{

	}

	AudioClip::~AudioClip()
	{
		mSound->release();
		mSound = nullptr;
	}

	HRESULT AudioClip::Load(const std::filesystem::path& _path)
	{
		if (false == AudioMgr::CreateSound(_path.string(), &mSound))
			return S_FALSE;

		mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

		return S_OK;
	}

	void AudioClip::Play()
	{
		if (mbLoop)
			mSound->setMode(FMOD_LOOP_NORMAL);
		else
			mSound->setMode(FMOD_LOOP_OFF);

		AudioMgr::SoundPlay(mSound, &mChannel);
	}

	void AudioClip::Stop()
	{
		mChannel->stop();
	}

	void AudioClip::Set3DAttributes(const math::Vector3 _pos, const math::Vector3 _vel)
	{
		FMOD_VECTOR fmodPos(_pos.x, _pos.y, _pos.z);
		FMOD_VECTOR fmodVel(_vel.x, _vel.y, _vel.z);

		mChannel->set3DAttributes(&fmodPos, &fmodVel);
	}
}
