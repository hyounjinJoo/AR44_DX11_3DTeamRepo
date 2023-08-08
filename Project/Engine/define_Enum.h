#pragma once

#include <limits.h>

namespace mh::define
{
	enum class eResult
	{
		Fail_NotImplemented = 0,

		//음수 ~ 0 = 실패
		Fail = INT_MIN,

		Fail_OpenFile,

		Fail_Compile,
		Fail_Create,

		Fail_Nullptr,

		Fail_MissingValue,

		Fail_Json,
		Fail_InValid,

		Fail_PathNotExist,

		//양수 = 성공
		//이 아래로 성공 관련 변수를 저장
		Success = 1
	};
	inline bool eResultSuccess(eResult _result) { return ((int)_result >= (int)eResult::Success); }
	inline bool eResultFail(eResult _result) { return ((int)_result < (int)eResult::Success); }


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

	enum class eProjectionType
	{
		None = -1,
		Perspective,
		Orthographic,
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
