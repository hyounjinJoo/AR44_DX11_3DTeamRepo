#pragma once
#include "define_GPU.h"
#include "SimpleMath.h"



namespace mh
{
	class ConstBuffer;
	class StructBuffer;
	class Texture;
	
	class Com_Camera;
	class GameObject;
	
	using namespace math;
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

	CBUFFER(SBufferCB, CBSLOT_SBUFFER)
	{
		UINT SBufferDataCount;
	};

	class MultiRenderTarget;
	class RenderMgr
	{
		friend class Application;
	public:
		static void Render();

		static ConstBuffer* GetConstBuffer(eCBType _Type) { return mConstBuffers[(int)_Type].get(); }
		static inline Com_Camera* GetMainCam() { return mMainCamera; }
		static ComPtr<ID3D11RasterizerState>	GetRasterizerState(eRSType _Type) { return mRasterizerStates[(int)_Type]; }
		static ComPtr<ID3D11BlendState>		GetBlendState(eBSType _Type) { return mBlendStates[(int)_Type]; }
		static ComPtr<ID3D11DepthStencilState> GetDepthStencilState(eDSType _Type) { return mDepthStencilStates[(int)_Type]; }
		
		static void SetMainCamera(Com_Camera* _pCam) { mMainCamera = _pCam; }
		static inline Com_Camera* GetCamera(eSceneType _Type, UINT _Idx);

		static void RegisterCamera(eSceneType _Type, Com_Camera* _pCam) { mCameras[(int)_Type].push_back(_pCam); }
		
		static void AddDebugMesh(const tDebugMesh& _DebugMesh) { mDebugMeshes.push_back(_DebugMesh); }

		static std::vector<tDebugMesh>& GetDebugMeshes() { return mDebugMeshes; }

		static void SetInspectorGameObject(GameObject* _pObj) { mInspectorGameObject = _pObj; }
		static GameObject* GetInspectorGameObject() { return mInspectorGameObject; }


		//Renderer
		static void PushLightAttribute(const tLightAttribute& lightAttribute) { mLights.push_back(lightAttribute); }


		static void BindLights();
		static void BindNoiseTexture();
		static void CopyRenderTarget();

	private:
		static bool CreateMultiRenderTargets();

		static void LoadDefaultMesh();
		static void LoadDefaultMaterial();
		static void LoadDefaultShader();
		static void LoadDefaultTexture();

		static void CreateSamplerStates();
		static void CreateRasterizerStates();
		static void CreateDepthStencilStates();
		static void CreateBlendStates();
		
		static void LoadBuffer();

		static void Init();
		static void Release();

	private:
		static Com_Camera* mMainCamera;
		static GameObject* mInspectorGameObject;

		static std::unique_ptr<ConstBuffer>		mConstBuffers[(UINT)eCBType::End];
		static ComPtr<ID3D11SamplerState>		mSamplerStates[(UINT)eSamplerType::End];
		static ComPtr<ID3D11RasterizerState>	mRasterizerStates[(UINT)eRSType::End];
		static ComPtr<ID3D11DepthStencilState>	mDepthStencilStates[(UINT)eDSType::End];
		static ComPtr<ID3D11BlendState>			mBlendStates[(UINT)eBSType::End];
		
		static std::vector<Com_Camera*>			mCameras[(UINT)eSceneType::End];
		static std::vector<tDebugMesh>			mDebugMeshes;

		static std::unique_ptr<MultiRenderTarget> mMultiRenderTargets[(UINT)eMRTType::End];

		static std::vector<tLightAttribute>		mLights;
		static std::unique_ptr<StructBuffer>	mLightsBuffer;

		static std::shared_ptr<Texture>			mPostProcessTexture;

		
	};


	inline Com_Camera* RenderMgr::GetCamera(eSceneType _Type, UINT _Idx)
	{
		Com_Camera* pCam = nullptr;
		if (mCameras[(int)_Type].size() > (size_t)_Idx)
			pCam = mCameras[(int)_Type][_Idx];

		return pCam;
	}
}


