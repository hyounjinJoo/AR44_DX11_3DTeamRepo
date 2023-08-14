#include "PCH_Engine.h"

#include "Animation2D.h"
#include "TimeMgr.h"
#include "RenderMgr.h"

#include "ConstBuffer.h"
#include "json-cpp\json.h"


namespace mh
{
	Animation2D::Animation2D()
		: IRes(eResourceType::Animation)
		, mAnimator(nullptr)
		, mAtlas(nullptr)
		, mSpriteSheet{}
		, mIndex(-1)
		, mTime(0.0f)
		, mbComplete(false)
	{

	}

	Animation2D::Animation2D(const Animation2D& _other)
		: IRes(_other)
		, mAnimator() //이건 이 애니메이션을 복사해서 가져가는 주인이 새로 설정해줘야함
		, mAnimationName(_other.mAnimationName)
		, mAtlas(_other.mAtlas) //Atlas == Texture -> 공유하는 리소스
		, mSpriteSheet(_other.mSpriteSheet) //vector도 value를 담고 있는 경우에는 그냥 복사하면 됨
		, mIndex(_other.mIndex)
		, mTime(_other.mTime)
		, mbComplete(_other.mbComplete)
	{
	}

	Animation2D::~Animation2D()
	{
	}

	eResult Animation2D::Load(const std::filesystem::path& _fileName)
	{
		return eResult();
	}

	eResult Animation2D::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = IRes::SaveJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}

		////Value가 들어있는 vector 저장하는법
		////1. json의 타입을 array value로 만들어준다.
		//(*_pJVal)[JSON_KEY(mSpriteSheet)] = Json::Value(Json::arrayValue);
		////2. 만들어진 데이터 컨테이너를 레퍼런스로 받아온다.
		//Json::Value& jVal = (*_pJVal)[JSON_KEY(mSpriteSheet)];
		//for (size_t i = 0; i < mSpriteSheet.size(); ++i)
		//{
		//	//3. 순회돌면서 하나씩 추가한다.
		//	jVal.append(Json::MHConvertWrite(mSpriteSheet[i]));
		//}
		Json::MHSaveVector(_pJVal, JSON_KEY_PAIR(mSpriteSheet));


		return eResult::Success;
	}

	eResult Animation2D::LoadJson(const Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = IRes::LoadJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}
		const Json::Value& jVal = (*_pJVal);

		auto SpriteData = Json::MHGetJsonVector(_pJVal, JSON_KEY_PAIR(mSpriteSheet));
		mSpriteSheet = std::move(SpriteData);

		


		return eResult::Success;
	}

	uint Animation2D::Update()
	{
		if (mbComplete)
			return -1;

		// 시간 체크
		mTime += TimeMgr::DeltaTime();

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

	void Animation2D::FixedUpdate()
	{

	}

	void Animation2D::Render()
	{
	}

	void Animation2D::Create(const std::string_view _name
		, std::shared_ptr<Texture> _atlas
		, float2 _leftTop, float2 _size, float2 _offset
		, uint _spriteLegth, float _duration)
	{
		mAnimationName = _name;

		mAtlas = _atlas;
		float width = static_cast<float>(_atlas->GetWidth());
		float height = static_cast<float>(_atlas->GetHeight());

		for (int index = 0; index < static_cast<int>(_spriteLegth); index++)
		{
			// API 와는 다르게 0~1 사이의 비율좌표로 위치를 표현해야한다.
			tSprite sprite = {};
			sprite.LeftTop = float2((_leftTop.x + (_size.x * static_cast<float>(index))) / width
				, (_leftTop.y) / height);
			sprite.Size = float2(_size.x / width, _size.y / height);
			sprite.Offset = _offset;
			sprite.Duration = _duration;
			sprite.AtlasSize = float2(200.0f / width, 200.0f / height);

			mSpriteSheet.push_back(sprite);
		}

	}

	void Animation2D::BindShader()
	{
		mAtlas->BindDataSRV(Register_t_atlasTexture, eShaderStageFlag::PS);

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::Animation2D);

		tCB_Animation2D info = {};
		info.animationType = (uint)eDimensionType::_2D;
		info.leftTop = mSpriteSheet[mIndex].LeftTop;
		info.offset = mSpriteSheet[mIndex].Offset;
		info.spriteSize = mSpriteSheet[mIndex].Size;
		info.atlasSize = mSpriteSheet[mIndex].AtlasSize;

		cb->SetData(&info);
		cb->BindData(eShaderStageFlag::PS);
	}

	void Animation2D::Reset()
	{
		mTime = 0.0f;
		mIndex = 0;
		mbComplete = false;
	}

	void Animation2D::Clear()
	{
		//Texture clear
		Texture::Clear(12);

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::Animation2D);

		tCB_Animation2D info = {};
		info.animationType = (uint)define::eDimensionType::NOT_SET;

		cb->SetData(&info);
		cb->BindData(eShaderStageFlag::PS);
	}

}
