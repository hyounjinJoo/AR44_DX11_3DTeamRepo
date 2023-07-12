#include "yaEnums.h"

namespace ya::enums
{


	//enum class eComponentType
	//{
	//	None,
	//	Transform, // 위치 데이터 수정하는 컴포넌트
	//	Camera,
	//	Mesh,
	//	Collider,
	//	//Collider2,
	//	MeshRenerer,
	//	SpriteRenderer,
	//	Animator,
	//	ParticleSystem,
	//	Light,
	//	UI,
	//	//FadeOut,FadeIn
	//	Script,
	//	End,
	//};
	
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