#pragma once
#include "Entity.h"
#include "Texture.h"

namespace mh
{
	using mh::Texture;

	class Animation2D : public IRes
	{
	public:
		struct tSprite
		{
			float2 LeftTop;			// 좌측 상단 좌표
			float2 Size;				// 좌측상단부터 잘라낼 가로 세로의 픽셀 길이
			float2 Offset;			// 렌더링 위치 조정하기위한 좌표
			float2 AtlasSize;		// 텍스처 이미지의 크기
			float Duration;					// 프레임간에 시간 간격

			tSprite()
				: LeftTop(0.0f, 0.0f)
				, Size(0.0f, 0.0f)
				, Offset(0.0f, 0.0f)
				, AtlasSize(0.0f)
				, Duration(0.1f)
			{

			}
		};

		Animation2D();

		Animation2D(const Animation2D& _other);
		CLONE(Animation2D);

		virtual ~Animation2D();

		virtual eResult Load(const std::filesystem::path& _fileName) override;
		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		uint Update();
		void FixedUpdate();
		void Render();

		void Create(const std::string_view _name, std::shared_ptr<Texture> _atlas
			, float2 _leftTop, float2 _size, float2 _offset
			, uint _spriteLegth, float _duration);

		//아틀라스를 균일하게 조각조각 내줌
		void CreateXY(const std::string_view _name, std::shared_ptr<Texture> _atlas, UINT _uColTotal, UINT _uRowTotal, float _duration);

		void BindShader();
		void Reset();
		void Clear();

		bool IsComplete() const { return mbComplete; }
		const std::string& GetAnimationName() const { return mAnimationName; }
		inline float2 GetSpriteSize(UINT _spriteIdx);

	private:
		class Com_Animator2D* mAnimator;
		std::string mAnimationName;
		std::shared_ptr<Texture> mAtlas;
		std::vector<tSprite> mSpriteSheet;
		int mIndex;
		float mTime;
		bool mbComplete;
	};

	inline float2 Animation2D::GetSpriteSize(UINT _spriteIdx)
	{
		float2 retSize{};
		if (_spriteIdx < (UINT)mSpriteSheet.size())
		{
			retSize = mSpriteSheet[_spriteIdx].AtlasSize;
		}
		return retSize;
	}
}
