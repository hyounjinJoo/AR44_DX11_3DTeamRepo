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
		Renderer,

		AudioSource,
		AudioListener,

		Scripts,

		END
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
				STRKEY_DECLARE(Com_DummyTransform);
				STRKEY_DECLARE(Com_Transform);

				STRKEY_DECLARE(Com_Animator2D);
				STRKEY_DECLARE(Com_Animator3D);
				STRKEY_DECLARE(Com_DummyAnimator);

				STRKEY_DECLARE(Com_AudioListener);
				STRKEY_DECLARE(Com_AudioSource);
				STRKEY_DECLARE(Com_Camera);
				STRKEY_DECLARE(Com_Light3D);
				STRKEY_DECLARE(Com_Renderer_Mesh);
				STRKEY_DECLARE(Com_Renderer_3DAnimMesh);
				STRKEY_DECLARE(Com_Renderer_ParticleSystem);
				STRKEY_DECLARE(Com_Renderer_Sprite);
				

				STRKEY_DECLARE(Script_Player);
				STRKEY_DECLARE(Script_Camera);
			}
		}
	}
}
