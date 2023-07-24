#pragma once
#include "Entity.h"
#include "Texture.h"

namespace mh
{
	class Animation : public Entity
	{
	public:
		struct tSprite
		{
			math::Vector2 LeftTop;	// 좌측 상단 좌표
			math::Vector2 Size;		// 좌측상단부터 잘라낼 가로 세로의 픽셀 길이
			math::Vector2 Offset;		// 렌더링 위치 조정하기위한 좌표
			math::Vector2 AtlasSize;  // 텍스처 이미지의 크기
			float Duration;		// 프레임간에 시간 간격

			tSprite()
				: LeftTop(0.0f, 0.0f)
				, Size(0.0f, 0.0f)
				, Offset(0.0f, 0.0f)
				, AtlasSize(0.0f)
				, Duration(0.1f)
			{

			}
		};

		Animation();
		~Animation();

		UINT Update();
		void FixedUpdate();
		void Render();

		void Create(const std::wstring& _name, std::shared_ptr<graphics::Texture> _atlas
			, math::Vector2 _leftTop, math::Vector2 _size, math::Vector2 _offset
			, UINT _spriteLegth, float _duration);

		void BindShader();
		void Reset();
		void Clear();

		bool IsComplete() const { return mbComplete; }
		const std::wstring& GetAnimationName() const { return mAnimationName; }

	private:
		class Animator* mAnimator;
		std::wstring mAnimationName;
		std::shared_ptr<graphics::Texture> mAtlas;
		std::vector<tSprite> mSpriteSheet;
		int mIndex;
		float mTime;
		bool mbComplete;
	};
}
