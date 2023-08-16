#pragma once

#include "SimpleMath.h"

//전방선언
namespace FMOD
{
	namespace Studio
	{
		class System;
	}
	class Sound;
	class System;
	class Channel;
}

namespace mh
{
	class AudioMgr
	{
		friend class Application;
	public:
		
		static bool CreateSound(const std::filesystem::path& _fullPath, FMOD::Sound** _sound);
		static void SoundPlay(FMOD::Sound* _sound, FMOD::Channel** _channel);
		static void Set3DListenerAttributes(const float3* _POS, const float3* _VEL,
											const float3* _forward, const float3* _up);

	private:
		static void Init();
		static void Release();

	private:
		static FMOD::Studio::System* mSystem;
		static FMOD::System* mCoreSystem;
	};
}
