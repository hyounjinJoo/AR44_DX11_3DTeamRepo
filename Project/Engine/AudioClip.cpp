#include "PCH_Engine.h"
#include "AudioClip.h"

#include "Com_Transform.h"
#include "PathMgr.h"

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

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

	eResult AudioClip::Load(const std::filesystem::path& _path)
	{
		std::filesystem::path FullPath = PathMgr::GetRelResourcePath(GetResType());
		FullPath /= _path;


		if (false == AudioMgr::CreateSound(FullPath, &mSound))
			return eResult::Fail_Create;

		mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

		return eResult::Success;
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

	void AudioClip::Set3DAttributes(const float3 _pos, const float3 _vel)
	{
		FMOD_VECTOR fmodPos(_pos.x, _pos.y, _pos.z);
		FMOD_VECTOR fmodVel(_vel.x, _vel.y, _vel.z);

		mChannel->set3DAttributes(&fmodPos, &fmodVel);
	}
}
