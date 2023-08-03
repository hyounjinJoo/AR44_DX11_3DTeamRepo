#pragma once

#include "SimpleMath.h"

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace mh
{
	class AudioMgr
	{
		friend class Application;
	public:
		
		static bool CreateSound(const std::filesystem::path& _fullPath, FMOD::Sound** _sound);
		static void SoundPlay(FMOD::Sound* _sound, FMOD::Channel** _channel);
		static void Set3DListenerAttributes(const math::Vector3* _POS, const math::Vector3* _VEL,
											const math::Vector3* _forward, const math::Vector3* _up);

	private:
		static void Init();
		static void Release();

	private:
		static FMOD::Studio::System* mSystem;
		static FMOD::System* mCoreSystem;
	};
}
