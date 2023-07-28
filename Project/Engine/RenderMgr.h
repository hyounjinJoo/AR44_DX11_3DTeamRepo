#pragma once
#include "Singleton.h"

#include "define_GPU.h"
#include "SimpleMath.h"


namespace mh
{
	namespace GPU
	{
		class ConstBuffer;
		class StructBuffer;
		class Texture;
	}
	
	class Com_Camera;
	class GameObject;
	
	using namespace math;
	using namespace GPU;
	using namespace Microsoft::WRL;


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



	
	class RenderMgr :
		public Singleton<RenderMgr>
	{
		SINGLETON(RenderMgr);

	public:
		void Initialize();
		void Render();

		ConstBuffer* GetConstBuffer(eCBType _Type) { return mConstBuffers[(int)_Type]; }
		inline Com_Camera* GetMainCam() { return mMainCamera; }
		ComPtr<ID3D11RasterizerState>	GetRasterizerState(eRSType _Type) { return mRasterizerStates[(int)_Type]; }
		ComPtr<ID3D11BlendState>		GetBlendState(eBSType _Type) { return mBlendStates[(int)_Type]; }
		ComPtr<ID3D11DepthStencilState> GetDepthStencilState(eDSType _Type) { return mDepthStencilStates[(int)_Type]; }
		
		void SetMainCamera(Com_Camera* _pCam) { mMainCamera = _pCam; }
		void RegisterCamera(eSceneType _Type, Com_Camera* _pCam) { mCameras[(int)_Type].push_back(_pCam); }
		Com_Camera* GetCamera(eSceneType _Type, UINT _Idx);
		void AddDebugMesh(const tDebugMesh& _DebugMesh) { mDebugMeshes.push_back(_DebugMesh); }

		std::vector<tDebugMesh>& GetDebugMeshes() { return mDebugMeshes; }

		void SetInspectorGameObject(GameObject* _pObj) { mInspectorGameObject = _pObj; }
		GameObject* GetInspectorGameObject() const { return mInspectorGameObject; }


		//Renderer
		void PushLightAttribute(const GPU::tLightAttribute& lightAttribute) { mLights.push_back(lightAttribute); }


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

	private:
		Com_Camera* mMainCamera;

		ConstBuffer*						mConstBuffers[(UINT)eCBType::End];
		ComPtr<ID3D11SamplerState>			mSamplerStates[(UINT)eSamplerType::End];
		ComPtr<ID3D11RasterizerState>		mRasterizerStates[(UINT)eRSType::End];
		ComPtr<ID3D11DepthStencilState>		mDepthStencilStates[(UINT)eDSType::End];
		ComPtr<ID3D11BlendState>			mBlendStates[(UINT)eBSType::End];
		
		std::vector<Com_Camera*>			mCameras[(UINT)eSceneType::End];
		std::vector<tDebugMesh>				mDebugMeshes;
		std::vector<tLightAttribute>		mLights;
		StructBuffer*						mLightsBuffer;

		std::shared_ptr<Texture>			mPostProcessTexture;

		GameObject*							mInspectorGameObject;
	};


	inline Com_Camera* RenderMgr::GetCamera(eSceneType _Type, UINT _Idx)
	{
		Com_Camera* pCam = nullptr;
		if (mCameras[(int)_Type].size() > (size_t)_Idx)
			pCam = mCameras[(int)_Type][_Idx];

		return pCam;
	}
}


