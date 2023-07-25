#pragma once

#include "define_Macro.h"
#include "define_Enum.h"

namespace mh::define
{
	STRKEY ComponentPrefix = "Com_";

	enum class eComponentType
	{
		UNKNOWN = -1,

		Collider,
		Animator,
		Light,
		Camera,

		AudioSource,
		AudioListener,

		Renderer,

		Scripts,

		END
		
		////Transform, // 위치 데이터 수정하는 컴포넌트
		//Com_Camera,
		//Mesh,
		//Collider,
		////Collider2,
		//Renderer,
		////Com_Renderer_Mesh,
		////Com_Renderer_Sprite,
		//Com_Animator,
		//Com_Renderer_ParticleSystem,
		//Com_AudioListener,
		//AudioSource,
		//Com_Light,
		//UI,
		////FadeOut,FadeIn
		//IScript,
		//End,

	};

	STRKEY ArrComName[(int)eComponentType::END] =
	{
		"Collider",
		"Animator",
		"Light",
		"Camera",

		"AudioSource",
		"AudioListener",

		"Renderer",

		"Scripts"
	};


	namespace Default
	{
		namespace com
		{
			STRKEY_DECLARE(Com_Animator);
			STRKEY_DECLARE(Com_Camera);
			STRKEY_DECLARE(Com_Light);
		}
	}
}
