#pragma once
#include "IRes.h"
#include "Fmod.h"

namespace mh
{
	namespace stdfs = std::filesystem;

	class AudioClip : public IRes
	{
	public:
		AudioClip();
		~AudioClip();

		virtual HRESULT Load(const std::filesystem::path& _path) override;

		void Play();
		void Stop();
		void Set3DAttributes(const math::Vector3 _pos, const math::Vector3 _vel);
		void SetLoop(bool _loop) { mbLoop = _loop; }

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		float mMinDistance;
		float mMaxDistance;
		bool mbLoop;
	};
}
