#pragma once

namespace mh::define
{
	enum class eResult
	{
		Fail_NotImplemented,

		Fail = INT_MIN,

		Fail_OpenFile,

		Fail_Compile,
		Fail_Create,

		Fail_Nullptr,

		Fail_MissingValue,

		Fail_Json,
		Fail_InValid,

		//이 아래로 성공 관련 변수를 저장
		Success = 1
	};
	inline bool eResultSuccess(eResult _result) { return ((int)_result >= (int)eResult::Success); }
	inline bool eResultFail(eResult _result) { return ((int)_result < (int)eResult::Success); }

	enum class eSceneType
	{
		Tilte,
		Play,
		End,
	};

	enum class eLayerType
	{
		None = 0,
		Com_Camera,
		Grid,
		Monster,
		Player,

		tParticle,
		PostProcess,
		UI,
		End = 16,
	};


	enum class eColliderType
	{
		None,
		Rect,
		Circle,
		Box,
		Sphere,
		End,
	};

	enum class eAnimationType
	{
		None,
		SecondDimension,
		ThirdDimension,
		End,
	};

	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		End,
	};
}
