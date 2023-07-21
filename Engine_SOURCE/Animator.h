#pragma once
#include "IComponent.h"
#include "Animation.h"

namespace mh
{
	using namespace mh::math;

	class Animator : public IComponent
	{
	public:
		struct tEvents
		{
			struct tEvent
			{
				void operator=(std::function<void()> _func)
				{
					Event = std::move(_func);
				}
				void operator()()
				{
					if (Event)
						Event();
				}

				std::function<void()> Event;
			};

			tEvent StartEvent;
			tEvent CompleteEvent;
			tEvent EndEvent;

			std::vector<tEvent> Events;
		};

		Animator();
		~Animator();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		bool Create(const std::wstring& _name
			, std::shared_ptr<graphics::Texture> _atlas
			, Vector2 _leftTop, Vector2 _size
			, Vector2 _offset
			, UINT _spriteLength
			, float _duration);

		Animation* FindAnimation(const std::wstring& _name) const;
		tEvents* FindEvents(const std::wstring& _name) const; 
		void Play(const std::wstring& _name, bool _loop = true);

		void Binds();
		void Clear();

		std::function<void()>& GetStartEvent(const std::wstring& _name) const;
		std::function<void()>& GetCompleteEvent(const std::wstring& _name) const;
		std::function<void()>& GetEndEvent(const std::wstring& _name) const;
		std::function<void()>& GetEvent(const std::wstring& _name, UINT _index) const;

	private:

		std::map<std::wstring, Animation*> mAnimations;
		std::map<std::wstring, tEvents*> mEvents;
		Animation* mActiveAnimation;
		bool mbLoop;
	};
}
