#include "Animation.h"
#include "Time.h"
#include "Renderer.h"

namespace mh
{
	Animation::Animation()
		: mAnimator(nullptr)
		, mAtlas(nullptr)
		, mSpriteSheet{}
		, mIndex(-1)
		, mTime(0.0f)
		, mbComplete(false)
	{

	}

	Animation::~Animation()
	{
	}

	UINT Animation::Update()
	{
		if (mbComplete)
			return -1;

		// 시간 체크
		mTime += Time::DeltaTime();

		// 누적 시간이 해당 프레임의 유지시간을 넘어서면 다음프레임으로 이동
		if (mSpriteSheet[mIndex].Duration < mTime)
		{
			mTime = 0.0f;
			++mIndex;
			if (mSpriteSheet.size() <= mIndex)
			{
				mbComplete = true;
				mIndex = static_cast<int>(mSpriteSheet.size()) - 1;
			}

			return mIndex;
		}

		return -1;
	}

	void Animation::FixedUpdate()
	{

	}

	void Animation::Render()
	{
	}

	void Animation::Create(const std::wstring& _name
		, std::shared_ptr<graphics::Texture> _atlas
		, math::Vector2 _leftTop, math::Vector2 _size, math::Vector2 _offset
		, UINT _spriteLegth, float _duration)
	{
		mAnimationName = _name;

		mAtlas = _atlas;
		float width = static_cast<float>(_atlas->GetWidth());
		float height = static_cast<float>(_atlas->GetHeight());

		for (int index = 0; index < static_cast<int>(_spriteLegth); index++)
		{
			// API 와는 다르게 0~1 사이의 비율좌표로 위치를 표현해야한다.
			tSprite sprite = {};
			sprite.LeftTop = math::Vector2((_leftTop.x + (_size.x * static_cast<float>(index))) / width
				, (_leftTop.y) / height);
			sprite.Size = math::Vector2(_size.x / width, _size.y / height);
			sprite.Offset = _offset;
			sprite.Duration = _duration;
			sprite.AtlasSize = math::Vector2(200.0f / width, 200.0f / height);

			mSpriteSheet.push_back(sprite);
		}

	}

	void Animation::BindShader()
	{
		mAtlas->BindShaderResource(graphics::eShaderStage::PS, 12);

		graphics::ConstantBuffer* cb = renderer::constantBuffers[(UINT)graphics::eCBType::Animation];

		renderer::AnimationCB info = {};
		info.type = (UINT)enums::eAnimationType::SecondDimension;
		info.leftTop = mSpriteSheet[mIndex].LeftTop;
		info.offset = mSpriteSheet[mIndex].Offset;
		info.size = mSpriteSheet[mIndex].Size;
		info.atlasSize = mSpriteSheet[mIndex].AtlasSize;

		cb->SetData(&info);
		cb->Bind(graphics::eShaderStage::PS);
	}

	void Animation::Reset()
	{
		mTime = 0.0f;
		mIndex = 0;
		mbComplete = false;
	}

	void Animation::Clear()
	{
		//Texture clear
		graphics::Texture::Clear(12);

		graphics::ConstantBuffer* cb = renderer::constantBuffers[(UINT)graphics::eCBType::Animation];
		renderer::AnimationCB info = {};
		info.type = (UINT)enums::eAnimationType::None;

		cb->SetData(&info);
		cb->Bind(graphics::eShaderStage::PS);
	}

}
