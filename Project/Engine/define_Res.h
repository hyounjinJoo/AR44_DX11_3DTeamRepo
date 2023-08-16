#pragma once

#include "define_Macro.h"
#include "define_Enum.h"

namespace mh::define
{
	enum class eResourceType
	{
		UNKNOWN = -1,
		Mesh,
		MeshData,

		Texture,
		Material,
		//Sound,
		/*Font,*/
		Animation,

		AudioClip,
		Prefab,
		
		GraphicsShader,
		ComputeShader,
		END,

	};

	namespace strKey
	{
		STRKEY DirName_Content = "Res";
		//리소스 이름 겸 Res 폴더 내의 폴더명으로 사용
		STRKEY ArrResName[(int)eResourceType::END] =
		{
			"Mesh",
			"MeshData",

			"Texture",
			"Material",
			//"Sound",
			//"Font,",
			"Animation2D",

			"AudioClip",
			"Prefab",

			"Shader/Graphics",
			"Shader/Compute",
		};
		inline STRKEY GetResName(eResourceType _type) { return ArrResName[(int)_type]; }

		STRKEY DirName_ShaderCSO = "ShaderCSO";
		STRKEY Ext_ShaderCSO = ".cso";
		STRKEY Ext_MeshData = ".msh";
		STRKEY Ext_Material = ".json";

		namespace Default
		{
			namespace mesh
			{
				STRKEY_DECLARE(PointMesh);
				STRKEY_DECLARE(RectMesh);
				STRKEY_DECLARE(DebugRectMesh);
				STRKEY_DECLARE(CircleMesh);
				STRKEY_DECLARE(CubeMesh);
				STRKEY_DECLARE(SphereMesh);
			}

			namespace material
			{
				STRKEY_DECLARE(RectMaterial);
				STRKEY_DECLARE(SpriteMaterial);
				STRKEY_DECLARE(UIMaterial);
				STRKEY_DECLARE(GridMaterial);
				STRKEY_DECLARE(DebugMaterial);
				STRKEY_DECLARE(ParticleMaterial);
				STRKEY_DECLARE(Basic3DMaterial);
				STRKEY_DECLARE(PostProcessMaterial);
				STRKEY_DECLARE(DefferedMaterial);
				STRKEY_DECLARE(MergeMaterial);
				STRKEY_DECLARE(LightDirMaterial);
				STRKEY_DECLARE(LightPointMaterial);
			}

			namespace texture
			{
				STRKEY SmileTexture = "Smile.png";
				STRKEY DefaultSprite = "Light.png";
				STRKEY HPBarTexture = "HPBar.png";
				STRKEY CartoonSmoke = "particle\\CartoonSmoke.png";
				STRKEY noise_01 = "noise\\noise_01.png";
				STRKEY noise_02 = "noise\\noise_02.png";
				STRKEY noise_03 = "noise\\noise_03.jpg";
				STRKEY BasicCube = "Cube\\TILE_01.tga";
				STRKEY BasicCubeNormal = "Cube\\TILE_01_N.tga";
				STRKEY Brick = "Cube\\Brick.jpg";
				STRKEY Brick_N = "Cube\\Brick_N.jpg";
				STRKEY PaintTexture = "PaintTexture";
				STRKEY RenderTarget = "RenderTarget";
			}

			namespace shader
			{
				namespace graphics
				{
					STRKEY_DECLARE(RectShader);
					STRKEY_DECLARE(SpriteShader);
					STRKEY_DECLARE(UIShader);
					STRKEY_DECLARE(GridShader);
					STRKEY_DECLARE(DebugShader);
					STRKEY_DECLARE(PaintShader);
					STRKEY_DECLARE(ParticleShader);
					STRKEY_DECLARE(PostProcessShader);
					STRKEY_DECLARE(Basic3DShader);
					STRKEY_DECLARE(DefferedShader);
					//STRKEY_DECLARE(LightShader);
					STRKEY_DECLARE(LightDirShader);
					STRKEY_DECLARE(LightPointShader);
					STRKEY_DECLARE(MergeShader);
				}
				namespace compute
				{
					STRKEY_DECLARE(ParticleCS);
					STRKEY_DECLARE(PostProcessShader);
					STRKEY_DECLARE(BasicShader);
					STRKEY_DECLARE(Animation3D);
				}
			}
		}

	}



}
