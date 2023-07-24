#pragma once

#include "define_Macro.h"

#include "Enums.h"

namespace mh::define
{
	namespace strKey_Default
	{
		STRKEY_DECLARE(PointMesh);
		STRKEY_DECLARE(RectMesh);
		STRKEY_DECLARE(DebugRectMesh);
		STRKEY_DECLARE(CircleMesh);
		STRKEY_DECLARE(CubeMesh);
		STRKEY_DECLARE(SphereMesh);
	}


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
			"GraphicsShader/Graphics",
			"GraphicsShader/Compute",
			"AudioClip",
			"Script"
		};
	}
}
