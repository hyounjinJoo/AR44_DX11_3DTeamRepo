#pragma once

#include "Enums.h"

#define STRKEY constexpr const char*

namespace mh::define
{
	namespace DirName
	{
		STRKEY Resource = "Res";

		STRKEY ResPath[(int)enums::eResourceType::End] =
		{
			"Mesh",
			"Texture",
			"Material",
			"Sound",
			/*Font,*/
			"Prefab",
			"MeshData",
			"Shader/Graphics",
			"Shader/Compute",
			"AudioClip",
			"Script"
		};
	}
}
