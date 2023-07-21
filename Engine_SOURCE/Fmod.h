#pragma once
#include "EnginePCH.h"
#include "SimpleMath.h"

#include "..\\External\\Fmod\\Include\\fmod_studio.hpp"
#include "..\\External\\Fmod\\Include\\fmod.hpp"
#include "..\\External\\Fmod\\Include\\fmod_common.h"
#include "..\\External\\Fmod\\Include\\fmod_codec.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Fmod\\Library\\Debug\\fmodL_vc.lib")
#pragma comment(lib, "..\\External\\Fmod\\Library\\Debug\\fmodstudioL_vc.lib")
#else
#pragma comment(lib, "..\\External\\Fmod\\Library\\Debug\\fmod_vc.lib")
#pragma comment(lib, "..\\External\\Fmod\\Library\\Release\\fmodstudio_vc.lib")
#endif

namespace mh
{
	class Fmod
	{
	public:
		static void Initialize();
		static bool CreateSound(const std::string& _path, FMOD::Sound** _sound);
		static void SoundPlay(FMOD::Sound* _sound, FMOD::Channel** _channel);
		static void Set3DListenerAttributes(const math::Vector3* _POS, const math::Vector3* _VEL,
											const math::Vector3* _forward, const math::Vector3* _up);
		static void Release();

	private:
		static FMOD::Studio::System* mSystem;
		static FMOD::System* mCoreSystem;
	};
}
