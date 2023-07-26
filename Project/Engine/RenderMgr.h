#pragma once
#include "Singleton.h"

#include "define_GPU.h"
#include "SimpleMath.h"


namespace mh
{
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


	using namespace mh::math;
	using namespace mh::GPU;
	using namespace Microsoft::WRL;

	class ConstBuffer;
	class StructBuffer;
	class Com_Camera;
	class GameObject;
	
	class RenderMgr :
		public Singleton<RenderMgr>
	{
		SINGLETON(RenderMgr);

	public:
		void Initialize();
		void Render();
		void Release();

		//Renderer
		void PushLightAttribute(const GPU::tLightAttribute& lightAttribute);


		void BindLights();
		void BindNoiseTexture();
		void CopyRenderTarget();

	private:
		void LoadMesh();
		void LoadMaterial();
		void LoadShader();
		void SetupState();
		void LoadBuffer();
		void LoadTexture();
		void LoadMaterial();

	private:
		Com_Camera* mMainCamera;

		ConstBuffer*					mConstBuffers[(UINT)eCBType::End];
		ComPtr<ID3D11SamplerState>			mSamplerStates[(UINT)eSamplerType::End];
		ComPtr<ID3D11RasterizerState>		mRasterizerStates[(UINT)eRSType::End];
		ComPtr<ID3D11DepthStencilState>		mDepthStencilStates[(UINT)eDSType::End];
		ComPtr<ID3D11BlendState>			mBlendStates[(UINT)eBSType::End];
		
		std::vector<Com_Camera*>			mCameras;
		std::vector<tDebugMesh>				mDebugMeshes;
		std::vector<tLightAttribute>		mLights;
		StructBuffer*					mLightsBuffer;

		GameObject*							mInspectorGameObject;
	};
}


