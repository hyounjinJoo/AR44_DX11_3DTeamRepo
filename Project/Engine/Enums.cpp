#include "EnginePCH.h"

#include "Enums.h"

namespace mh::enums
{
	const char* charComponentType[(int)eComponentType::End] =
	{
		"None",
		"Transform",
		"Camera",
		"Mesh",
		"Collider",
		"MeshRenerer",
		"SpriteRenderer",
		"Animator",
		"ParticleSystem",
		"Light",
		"UI",
		"Script",
	};

	const wchar_t* wcharComponentType[(int)eComponentType::End] =
	{
		L"None",
		L"Transform",
		L"Camera",
		L"Mesh",
		L"Collider",
		L"MeshRenerer",
		L"SpriteRenderer",
		L"Animator",
		L"ParticleSystem",
		L"Light",
		L"UI",
		L"Script",
	};

	const char* charResourceType[(int)eResourceType::End] =
	{
		"Mesh",
		"Texture",
		"Material",
		"Sound",
		//"/*Font,*/
		"Prefab",
		"MeshData",
		"GraphicShader",
		"ComputeShader",
		"Script",
	};

	const wchar_t* wcharResourceType[(int)eResourceType::End] =
	{
		L"Mesh",
		L"Texture",
		L"Material",
		L"Sound",
		//"/*Font,*/
		L"Prefab",
		L"MeshData",
		L"GraphicShader",
		L"ComputeShader",
		L"Script",
	};
}