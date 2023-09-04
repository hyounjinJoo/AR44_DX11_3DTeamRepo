#pragma once

#include <limits.h>

namespace mh
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

		Fail_Empty,

		//양수 = 성공
		//이 아래로 성공 관련 변수를 저장
		Success = 1
	};
	inline bool eResultSuccess(eResult _result) { return ((int)_result >= (int)eResult::Success); }
	inline bool eResultFail(eResult _result) { return ((int)_result < (int)eResult::Success); }

	namespace define
	{
		enum class eLayerType
		{
			None = -1,
			Com_Camera,
			Grid,
			Stage,
			Monster,
			Player,

			tParticle,
			PostProcess,
			UI,

			END
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
			END,
		};

		enum class eLightType
		{
			Directional,
			Point,
			Spot,
			END,
		};

		enum class eDimensionType
		{
			NOT_SET,
			_2D,
			_3D
		};
	}
}
