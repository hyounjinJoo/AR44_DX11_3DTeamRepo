#include "PCH_Engine.h"

#include "Com_Animator2D.h"

#include "json-cpp\json.h"
#include "GameObject.h"

namespace mh
{
	Com_Animator2D::Com_Animator2D()
		: IAnimator(define::eDimensionType::_2D)
		, mAnimations{}
		, mEvents{}
		, mActiveAnimation(nullptr)
		, mbLoop(false)
	{

	}
	Com_Animator2D::Com_Animator2D(const Com_Animator2D& _other)
		: IAnimator(_other)
		//복사작업을 따로 해줘야할 경우 여기서는 일단 초기화해줌
		, mAnimations{}
		, mEvents{}
		, mActiveAnimation{}

		//bool 변수 = value이브로 바로 복사
		, mbLoop(_other.mbLoop)
	{

		//std::unordered_map<std::string, Animation*, define::tUmap_StringViewHasher, std::equal_to<>>	mAnimations;
		//이 부분 주의해야함 _other.mAnimation이랑 mAnimation이랑 헷갈릴때 많음
		for (const auto& iter : _other.mAnimations)
		{
			if (nullptr == iter.second)
				continue;

			//애니메이션을 복사해서 새로 받아온다.
			auto* AnimClone = iter.second->Clone();

			//애니메이션 이름이 비어있으면 에러 발생시킴
			if (AnimClone->GetAnimationName().empty())
			{
				SAFE_DELETE(AnimClone);
				ERROR_MESSAGE_W(L"애니메이션 이름이 존재하지 않습니다.");
			}
			else
			{
				//복사한 애니메이션을 내 애니메이션 목록에 집어넣음
				mAnimations.insert(std::make_pair(AnimClone->GetAnimationName(), AnimClone));


				//Animation* mActiveAnimation;
				//만약 재생중이던 애니메이션이 있는데
				if (_other.mActiveAnimation)
				{
					//그 애니메이션이랑 방금 복사한 애니메이션이랑 이름이 같을경우
					if (_other.mActiveAnimation->GetAnimationName() == AnimClone->GetAnimationName())
					{
						//여기도 동일하게 복사한 애니메이션 주소를 대입
						mActiveAnimation = AnimClone;
					}
				}
			}	
			//std::unordered_map<std::string, tEvents*, define::tUmap_StringViewHasher, std::equal_to<>> mEvents;
			//mEvents(노티파이 함수)의 경우 누가 호출했는지를 알수 없기때문에 그냥 복사하면 에러 날수도 있음
			//예를들어서 A 게임오브젝트에서 자신에게서 소리 나도록 설정했는데 B 오브젝트로 그냥 복사해서 가져오면
			//B오브젝트가 애니메이션 재생했는데 A 오브젝트에서 소리날수도 있다던가 그런거
			//이건 그래서 함수를 등록한 쪽에서 직접 다시 등록을 해줘야함

			//이런식으로 복사생성자를 구현해주면 됨
		}

	}

	Com_Animator2D::~Com_Animator2D()
	{
		for (auto anim : mAnimations)
		{
			delete anim.second;
			anim.second = nullptr;
		}

		for (auto evt : mEvents)
		{
			delete evt.second;
			evt.second = nullptr;
		}
	}
	eResult Com_Animator2D::SaveJson(Json::Value* _pJson)
	{



		return eResult::Success;
	}
	eResult Com_Animator2D::LoadJson(const Json::Value* _pJson)
	{
		return eResult();
	}
	void Com_Animator2D::Init()
	{
	}
	void Com_Animator2D::Update()
	{
		if (mActiveAnimation == nullptr)
			return;

		tEvents* events = FindEvents(mActiveAnimation->GetAnimationName());
		if (mActiveAnimation->IsComplete())
		{
			if (events)
				events->CompleteEvent();

			if (mbLoop)
				mActiveAnimation->Reset();
		}
		
		if (events) 
		{
			uint spriteIndex = mActiveAnimation->Update();
			if (spriteIndex != -1
				&& spriteIndex < events->Events.size()
				&& events->Events[spriteIndex].Event)
			{
				events->Events[spriteIndex].Event();
			}
		}

	}
	void Com_Animator2D::FixedUpdate()
	{
	}


	bool Com_Animator2D::Create(const std::string_view _name, std::shared_ptr<Texture> _atlas
		, float2 _leftTop, float2 _size, float2 _offset
		, uint _spriteLegth, float _duration)
	{
		if (_atlas == nullptr)
			return false;

		Animation2D* animation = FindAnimation(_name);
		if (animation != nullptr)
			return false;

		animation = new Animation2D();
		animation->Create(_name, _atlas, _leftTop, _size, _offset, _spriteLegth, _duration);

		mAnimations.insert(std::make_pair(_name, animation));

		tEvents* events = new tEvents();
		events->Events.resize(_spriteLegth);
		mEvents.insert(std::make_pair(_name, events));

		return true;
	}

	bool Com_Animator2D::CreateXY(const std::string_view _name, std::shared_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration)
	{
		if (_atlas == nullptr)
			return false;

		Animation2D* animation = FindAnimation(_name);
		if (animation != nullptr)
			return false;

		animation = new Animation2D();
		animation->CreateXY(_name, _atlas, _uColTotal, _uRowTotal, _duration);

		mAnimations.insert(std::make_pair(_name, animation));

		tEvents* events = new tEvents();
		events->Events.resize(_uColTotal * _uRowTotal);
		mEvents.insert(std::make_pair(_name, events));

		float2 size = animation->GetSpriteSize(0u);

		//사이즈 수정
		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>(); 
		if(tr)
			tr->SetSizeXY(animation->GetSpriteSize(0u));

		return true;
	}

	Animation2D* Com_Animator2D::FindAnimation(const std::string_view _name) const
	{
		const auto iter = mAnimations.find(_name);

		if (iter == mAnimations.end())
		{
			return nullptr;
		}

		return iter->second;
	}
	
	Com_Animator2D::tEvents* Com_Animator2D::FindEvents(const std::string_view _name) const
	{
		const auto iter
			= mEvents.find(_name);

		if (iter == mEvents.end())
		{
			return nullptr;
		}

		return iter->second;
	}
	void Com_Animator2D::Play(const std::string_view _name, bool _loop)
	{
		Animation2D* prevAnimation = mActiveAnimation;
		tEvents* events = nullptr;
		if (prevAnimation)
			events = FindEvents(prevAnimation->GetAnimationName());

		if (events)
			events->EndEvent();

		mActiveAnimation = FindAnimation(_name);
		mActiveAnimation->Reset();
		mbLoop = _loop;

		events = FindEvents(mActiveAnimation->GetAnimationName());

		if (events)
			events->StartEvent();
	}

	void Com_Animator2D::BindData()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->BindShader();
	}

	void Com_Animator2D::Clear()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->Clear();
	}



	std::function<void()>& Com_Animator2D::GetStartEvent(const std::string_view _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->StartEvent.Event;
	}
	std::function<void()>& Com_Animator2D::GetCompleteEvent(const std::string_view _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->CompleteEvent.Event;
	}
	std::function<void()>& Com_Animator2D::GetEndEvent(const std::string_view _name) const
	{
		tEvents* events = FindEvents(_name);

		return events->EndEvent.Event;
	}
	std::function<void()>& Com_Animator2D::GetEvent(const std::string_view _name, uint _index) const
	{
		tEvents* events = FindEvents(_name);

		return events->Events[_index].Event;
	}
}
