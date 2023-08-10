#pragma once
#include "IComponent.h"
#include "Animation.h"

#include "define_Struct.h"

namespace mh
{
	class Com_Animator : public IComponent
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

		Com_Animator();

		Com_Animator(const Com_Animator& _other);
		CLONE(Com_Animator);

		virtual ~Com_Animator();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		bool Create(const std::string_view _name
			, std::shared_ptr<Texture> _atlas
			, float2 _leftTop, float2 _size
			, float2 _offset
			, uint _spriteLength
			, float _duration);

		Animation* FindAnimation(const std::string_view _name) const;
		tEvents* FindEvents(const std::string_view _name) const; 
		void Play(const std::string_view _name, bool _loop = true);

		void Binds();
		void Clear();

		std::function<void()>& GetStartEvent(const std::string_view _name) const;
		std::function<void()>& GetCompleteEvent(const std::string_view _name) const;
		std::function<void()>& GetEndEvent(const std::string_view _name) const;
		std::function<void()>& GetEvent(const std::string_view _name, uint _index) const;

	private:
		std::unordered_map<std::string, Animation*, define::tUmap_StringViewHasher, std::equal_to<>>	mAnimations;
		std::unordered_map<std::string, tEvents*, define::tUmap_StringViewHasher, std::equal_to<>>		mEvents;
		Animation* mActiveAnimation;
		bool mbLoop;
	};
}
