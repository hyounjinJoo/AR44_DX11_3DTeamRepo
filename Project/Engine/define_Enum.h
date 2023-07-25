
#pragma once

namespace mh::define
{
	enum class eResult
	{
		Success,
		Fail_OpenFile,
		
		
		Fail_Compile,
		Fail_Create,

		Fail_NotImplemented
	};


	enum class eSceneType
	{
		Tilte,
		Play,
		End,
	};

	enum class eLayerType
	{
		None = 0,
		Camera,
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
