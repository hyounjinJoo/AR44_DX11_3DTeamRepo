#pragma once
#include "IComponent.h"
#include "AudioClip.h"

namespace mh
{
	class AudioSource : public IComponent
	{
	public:
		AudioSource();
		~AudioSource();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void Play();
		void Stop();
		void SetLoop(bool _loop);

		void SetClip(std::shared_ptr<AudioClip> _clip) { mAudioClip = _clip; }
		std::shared_ptr<AudioClip> GetClip() const { return mAudioClip; }

	private:
		std::shared_ptr<AudioClip> mAudioClip;
	};
}
