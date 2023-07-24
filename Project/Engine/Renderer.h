#pragma once
#include "EnumFlags.h"
#include "SimpleMath.h"
#include "GraphicDevice_DX11.h"

#include "Graphics.h"

#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "Light.h"
#include "StructedBuffer.h"



namespace mh::renderer
{
	using namespace mh::graphics;

	struct Vertex
	{
		Vector4 Pos;
		Vector4 Color;
		Vector2 UV;
		Vector3 Tangent;
		Vector3 BiNormal;
		Vector3 Normal;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix World;
		Matrix View;
		Matrix Projection;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		/*MaterialCB()
			: iData(0)
			, fData(0.f)
		{
		}*/
		UINT UsedAlbedo;
		UINT UsedNormal;

		UINT Padd0;
		UINT Padd1;
	};

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector4 CameraPosition;
		Vector2 CameraScale;
		Vector2 Resolution;
	};

	CBUFFER(AnimationCB, CBSLOT_ANIMATION)
	{
		Vector2 LeftTop;
		Vector2 Size;
		Vector2 Offset;
		Vector2 AtlasSize;

		UINT Type;
	};

	//CBSLOT_NUMBEROFLIGHT
	CBUFFER(LightCB, CBSLOT_NUMBEROFLIGHT)
	{
		UINT NumberOfLight;
	};

	CBUFFER(ParticleSystemCB, CBSLOT_PARTICLESYSTEM)
	{
		Vector4 WorldPosition;
		Vector4 StartColor;
		Vector4 StartSize;

		UINT MaxParticles;
		UINT SimulationSpace;
		float Radius;
		float StartSpeed;

		float StartLifeTime;
		float DeltaTime;
		float ElapsedTime; //누적시간
		int	Padding;
	};

	CBUFFER(NoiseCB, CBSLOT_NOISE)
	{
		Vector4 NoiseSize;
		float NoiseTime;
	};


	extern Vertex gVertices[4];
	extern Camera* gMainCamera;
	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> gSamplerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> gRasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> gDepthStencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> gBlendStates[];
	
	extern std::vector<Camera*> gCameras[];
	extern std::vector<tDebugMesh> gDebugMeshes;
	extern std::vector<graphics::tLightAttribute> gLights;
	extern graphics::StructedBuffer* gLightsBuffer;

	extern GameObject* gInspectorGameObject;

	void Initialize();
	void Render();
	void Release();

	//Renderer
	void PushLightAttribute(const graphics::tLightAttribute& lightAttribute);
	void BindLights();
	void BindNoiseTexture();
	void CopyRenderTarget();
}

