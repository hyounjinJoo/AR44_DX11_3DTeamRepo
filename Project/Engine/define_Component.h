#pragma once

#include "define_Macro.h"
#include "define_Enum.h"

namespace mh::define
{
	enum class eComponentType
	{
		UNKNOWN = -1 ,
		Transform, // 위치 데이터 수정하는 컴포넌트
		Camera,
		Mesh,
		Collider,
		//Collider2,
		MeshRenderer,
		SpriteRenderer,
		Animator,
		ParticleSystem,
		AudioListener,
		AudioSource,
		Light,
		UI,
		//FadeOut,FadeIn
		Script,
		End,
	};

	STRKEY ArrComName[(int)eComponentType::End] =
	{
		"Transform", // 위치 데이터 수정하는 컴포넌트
		"Camera",
		"Mesh",
		"Collider",
		//"Collider2",
		"MeshRenderer",
		"SpriteRenderer",
		"Animator",
		"ParticleSystem",
		"AudioListener",
		"AudioSource",
		"Light",
		"UI",
		//"FadeOut,FadeI",
		"Scrip",
	};
}
