#pragma once
#include "IComponent.h"
#include "AudioClip.h"

namespace mh
{
	class Com_AudioSource : public IComponent
	{
	public:
		Com_AudioSource();

		//복사할 변수가 있지만,
		//다 같이 공유하는 '리소스' 포인터이기 때문에 기본 복사 생성자로도 충분하다.
		Com_AudioSource(const Com_AudioSource& _other) = default;
		CLONE(Com_AudioSource);

		virtual ~Com_AudioSource();

		virtual void Init() override;
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
