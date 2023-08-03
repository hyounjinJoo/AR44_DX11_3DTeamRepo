#include "EnginePCH.h"
#include "AudioMgr.h"
#ifdef _DEBUG
#pragma comment(lib, "Fmod/Debug/fmodL_vc.lib")
#pragma comment(lib, "Fmod/Debug/fmodstudioL_vc.lib")
#else
#pragma comment(lib, "Fmod/Release/fmod_vc.lib")
#pragma comment(lib, "Fmod/Release/fmodstudio_vc.lib")
#endif

#include "AtExit.h"

namespace mh
{
	FMOD::Studio::System* AudioMgr::mSystem = nullptr;
	FMOD::System* AudioMgr::mCoreSystem = nullptr;

	void AudioMgr::Init()
	{
		AtExit::AddFunc(Release);

		void* extraDriverData = NULL;

		FMOD::Studio::System::create(&mSystem);

		// The example Studio project is authored for 5.1 sound, so set up the system output mode to match
		mSystem->getCoreSystem(&mCoreSystem);
		mCoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
		
		mSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
	}

	void AudioMgr::Release()
	{
		mSystem->release();
		mSystem = nullptr;
	}

	bool AudioMgr::CreateSound(const std::filesystem::path& _fullPath,  FMOD::Sound** sound)
	{
		bool bResult = false;
		if (FMOD_OK == mCoreSystem->createSound(_fullPath.string().c_str(), FMOD_3D, 0, sound))
		{
			bResult = true;
		}

		return bResult;
	}

	void AudioMgr::SoundPlay(FMOD::Sound* sound, FMOD::Channel** channel)
	{
		mCoreSystem->playSound(sound, 0, false, channel);
	}

	void AudioMgr::Set3DListenerAttributes(const math::Vector3* _position, const math::Vector3* _velocity,
									   const math::Vector3* _forward, const math::Vector3* _up)
	{
		FMOD_VECTOR fmodPosition(_position->x, _position->y, _position->z);
		FMOD_VECTOR fmodVelocity(_velocity->x, _velocity->y, _velocity->z);
		FMOD_VECTOR fmodForward(_forward->x, _forward->y, _forward->z);
		FMOD_VECTOR fmodUp(_up->x, _up->y, _up->z);

		mCoreSystem->set3DListenerAttributes(0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp);
	}




}
