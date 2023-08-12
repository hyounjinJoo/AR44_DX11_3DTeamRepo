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


	class MultiRenderTarget;
	class RenderMgr
	{
		friend class Application;
		friend class GPUMgr;
	public:
		static void Render();

		static ConstBuffer* GetConstBuffer(eCBType _Type) { return mConstBuffers[(int)_Type].get(); }

		//static void SetDataToConstBuffer(eCBType _Type, void* _pData, uint _dataCount = 1u);

		static inline Com_Camera* GetMainCam() { return mMainCamera; }
		static ComPtr<ID3D11RasterizerState>	GetRasterizerState(eRSType _Type) { return mRasterizerStates[(int)_Type]; }
		static ComPtr<ID3D11BlendState>		GetBlendState(eBSType _Type) { return mBlendStates[(int)_Type]; }
		static ComPtr<ID3D11DepthStencilState> GetDepthStencilState(eDSType _Type) { return mDepthStencilStates[(int)_Type]; }
		
		static void SetMainCamera(Com_Camera* _pCam) { mMainCamera = _pCam; }
		static inline Com_Camera* GetCamera(uint _Idx);

		static void RegisterCamera(Com_Camera* _pCam) { mCameras.push_back(_pCam); }
		
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
		static void UpdateGlobalCBuffer();

		//해상도 변경 시 GPUMgr에서 호출
		static bool SetResolution(UINT _ResolutionX, UINT _ResolutionY);

		static bool CreateMultiRenderTargets(UINT _ResolutionX, UINT _ResolutionY);


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

		static std::unique_ptr<ConstBuffer>		mConstBuffers[(uint)eCBType::END];
		static ComPtr<ID3D11SamplerState>		mSamplerStates[(uint)eSamplerType::END];
		static ComPtr<ID3D11RasterizerState>	mRasterizerStates[(uint)eRSType::END];
		static ComPtr<ID3D11DepthStencilState>	mDepthStencilStates[(uint)eDSType::END];
		static ComPtr<ID3D11BlendState>			mBlendStates[(uint)eBSType::END];
		
		static std::vector<Com_Camera*>			mCameras;
		static std::vector<tDebugMesh>			mDebugMeshes;

		static std::unique_ptr<MultiRenderTarget> mMultiRenderTargets[(uint)eMRTType::END];

		static std::vector<Com_Light*>			mLights;
		static std::vector<tLightAttribute>		mLightAttributes;
		static std::unique_ptr<StructBuffer>	mLightsBuffer;

		static std::shared_ptr<Texture>			mPostProcessTexture;
		static std::shared_ptr<Texture>			mNoiseTex;
	};


	inline Com_Camera* RenderMgr::GetCamera(uint _Idx)
	{
		Com_Camera* pCam = nullptr;
		if (mCameras.size() > (size_t)_Idx)
			pCam = mCameras[_Idx];

		return pCam;
	}

	inline MultiRenderTarget* RenderMgr::GetMultiRenderTarget(eMRTType _Type)
	{
		return mMultiRenderTargets[(int)_Type].get();
	}
}


