#pragma once
#include "define_GPU.h"
#include "SimpleMath.h"

namespace mh
{
	class ConstBuffer;
	class StructBuffer;
	class Texture;
	
	class Com_Camera;

	class Com_Light;
	class GameObject;
	
	
	using namespace mh::define;
	using namespace Microsoft::WRL;
	


	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		MATRIX World;
		MATRIX InverseWorld;
		MATRIX View;
		MATRIX InverseView;
		MATRIX Projection;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		/*MaterialCB()
			: iData(0)
			, fData(0.f)
		{
		}*/
		uint UsedAlbedo;
		uint UsedNormal;

		uint Padd0;
		uint Padd1;
	};

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		float4 CameraPosition;
		float2 CameraScale;
		float2 Resolution;
	};

	CBUFFER(AnimationCB, CBSLOT_ANIMATION)
	{
		float2 LeftTop;
		float2 Size;
		float2 Offset;
		float2 AtlasSize;

		uint Type;
	};

	//CBSLOT_NUMBEROFLIGHT
	CBUFFER(LightCB, CBSLOT_NUMBEROFLIGHT)
	{
		uint NumberOfLight;
		uint IndexOfLight;
	};

	CBUFFER(ParticleSystemCB, CBSLOT_PARTICLESYSTEM)
	{
		float4 WorldPosition;
		float4 StartColor;
		float4 StartSize;

		uint MaxParticles;
		uint SimulationSpace;
		float Radius;
		float StartSpeed;

		float StartLifeTime;
		float DeltaTime;
		float ElapsedTime; //누적시간
		int	Padding;
	};

	CBUFFER(NoiseCB, CBSLOT_NOISE)
	{
		float4 NoiseSize;
		float NoiseTime;
	};

	CBUFFER(SBufferCB, CBSLOT_SBUFFER)
	{
		uint SBufferDataCount;
	};

	class MultiRenderTarget;
	class RenderMgr
	{
		friend class Application;
	public:
		static void Render();

		static ConstBuffer* GetConstBuffer(eCBType _Type) { return mConstBuffers[(int)_Type].get(); }

		//static void SetDataToConstBuffer(eCBType _Type, void* _pData, uint _dataCount = 1u);

		static inline Com_Camera* GetMainCam() { return mMainCamera; }
		static ComPtr<ID3D11RasterizerState>	GetRasterizerState(eRSType _Type) { return mRasterizerStates[(int)_Type]; }
		static ComPtr<ID3D11BlendState>		GetBlendState(eBSType _Type) { return mBlendStates[(int)_Type]; }
		static ComPtr<ID3D11DepthStencilState> GetDepthStencilState(eDSType _Type) { return mDepthStencilStates[(int)_Type]; }
		
		static void SetMainCamera(Com_Camera* _pCam) { mMainCamera = _pCam; }
		static inline Com_Camera* GetCamera(eSceneType _Type, uint _Idx);

		static void RegisterCamera(eSceneType _Type, Com_Camera* _pCam) { mCameras[(int)_Type].push_back(_pCam); }
		
		static void AddDebugMesh(const tDebugMesh& _DebugMesh) { mDebugMeshes.push_back(_DebugMesh); }

		static std::vector<tDebugMesh>& GetDebugMeshes() { return mDebugMeshes; }

		static void SetInspectorGameObject(GameObject* _pObj) { mInspectorGameObject = _pObj; }
		static GameObject* GetInspectorGameObject() { return mInspectorGameObject; }

		inline static MultiRenderTarget* GetMultiRenderTarget(eMRTType _Type);

		//Renderer
		static void PushLightAttribute(const tLightAttribute& lightAttribute) { mLightAttributes.push_back(lightAttribute); }
		static void AddLight(Com_Light* _pComLight) { if (_pComLight) mLights.push_back(_pComLight); }
		static void RemoveLight(Com_Light* _pComLight);

		static const std::vector<Com_Light*>& GetLights() { return mLights; }


		static void BindLights();
		static void BindNoiseTexture();
		static void CopyRenderTarget();

		static void ClearMultiRenderTargets();

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

		static std::unique_ptr<ConstBuffer>		mConstBuffers[(uint)eCBType::End];
		static ComPtr<ID3D11SamplerState>		mSamplerStates[(uint)eSamplerType::End];
		static ComPtr<ID3D11RasterizerState>	mRasterizerStates[(uint)eRSType::End];
		static ComPtr<ID3D11DepthStencilState>	mDepthStencilStates[(uint)eDSType::End];
		static ComPtr<ID3D11BlendState>			mBlendStates[(uint)eBSType::End];
		
		static std::vector<Com_Camera*>			mCameras[(uint)eSceneType::End];
		static std::vector<tDebugMesh>			mDebugMeshes;

		static std::unique_ptr<MultiRenderTarget> mMultiRenderTargets[(uint)eMRTType::End];

		static std::vector<Com_Light*>			mLights;
		static std::vector<tLightAttribute>		mLightAttributes;
		static std::unique_ptr<StructBuffer>	mLightsBuffer;

		static std::shared_ptr<Texture>			mPostProcessTexture;
	};


	inline Com_Camera* RenderMgr::GetCamera(eSceneType _Type, uint _Idx)
	{
		Com_Camera* pCam = nullptr;
		if (mCameras[(int)_Type].size() > (size_t)_Idx)
			pCam = mCameras[(int)_Type][_Idx];

		return pCam;
	}

	inline MultiRenderTarget* RenderMgr::GetMultiRenderTarget(eMRTType _Type)
	{
		return mMultiRenderTargets[(int)_Type].get();
	}
}


