#pragma once
#include "GameResource.h"
#include "Fmod.h"

namespace mh
{
	class AudioClip : public GameResource
	{
	public:
		AudioClip();
		~AudioClip();

		virtual HRESULT Load(const std::wstring& _path) override;

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

