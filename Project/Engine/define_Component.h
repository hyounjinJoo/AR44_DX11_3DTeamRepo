#pragma once

#include "define_Macro.h"
#include "define_Enum.h"

namespace mh::define
{
	STRKEY ComponentPrefix = "Com_";

	enum class eComponentType
	{
		UNKNOWN = -1,

		Transform,
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
		//Com_Animator2D,
		//Com_Renderer_ParticleSystem,
		//Com_AudioListener,
		//Com_AudioSource,
		//Com_Light,
		//UI,
		////FadeOut,FadeIn
		//IScript,
		//End,

	};

	namespace strKey
	{
		STRKEY ArrComName[(int)eComponentType::END] =
		{
			"Transform",
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
				STRKEY_DECLARE(Com_Animator2D);
				STRKEY_DECLARE(Com_Animator3D);
				STRKEY_DECLARE(Com_AudioListener);
				STRKEY_DECLARE(Com_AudioSource);
				STRKEY_DECLARE(Com_Camera);
				STRKEY_DECLARE(Com_Light);
				STRKEY_DECLARE(Com_Renderer_Mesh);
				STRKEY_DECLARE(Com_Renderer_ParticleSystem);
				STRKEY_DECLARE(Com_Renderer_Sprite);
				STRKEY_DECLARE(Com_Transform);
			}
		}
	}
}
