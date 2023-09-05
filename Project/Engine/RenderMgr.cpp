#include "PCH_Engine.h"
#include "RenderMgr.h"

#include "AtExit.h"

#include "Mesh.h"
#include "Material.h"
#include "ResMgr.h"

#include "ConstBuffer.h"
#include "PaintShader.h"
#include "ParticleShader.h"
#include "Animation3DShader.h"

#include "SceneMgr.h"

#include "ResMgr.h"
#include "TimeMgr.h"

#include "MultiRenderTarget.h"
#include "Application.h"

#include "Com_Light3D.h"

//컴파일된 쉐이더 헤더 모아놓은 헤더
#include "DefaultShaders.h"

extern mh::Application gApplication;

namespace mh
{
	Com_Camera*							RenderMgr::mMainCamera{};
	GameObject*							RenderMgr::mInspectorGameObject{};

	std::unique_ptr<ConstBuffer>		RenderMgr::mConstBuffers[(uint)eCBType::END]{};
	ComPtr<ID3D11SamplerState>			RenderMgr::mSamplerStates[(uint)eSamplerType::END]{};
	ComPtr<ID3D11RasterizerState>		RenderMgr::mRasterizerStates[(uint)eRSType::END]{};
	ComPtr<ID3D11DepthStencilState>		RenderMgr::mDepthStencilStates[(uint)eDSType::END]{};
	ComPtr<ID3D11BlendState>			RenderMgr::mBlendStates[(uint)eBSType::END]{};
	std::vector<Com_Camera*>			RenderMgr::mCameras{};
	std::vector<tDebugMesh>				RenderMgr::mDebugMeshes{};

	std::unique_ptr<MultiRenderTarget>	RenderMgr::mMultiRenderTargets[(uint)eMRTType::END]{};

	std::vector<Com_Light3D*>				RenderMgr::mLights{};
	std::vector<tLightAttribute>		RenderMgr::mLightAttributes{};
	std::unique_ptr<StructBuffer>		RenderMgr::mLightsBuffer{};
	std::shared_ptr<Texture>			RenderMgr::mPostProcessTexture{};
	

	void RenderMgr::Init()
	{
		AtExit::AddFunc(Release);

		//GPUMgr에서 담당
		//if (false == CreateMultiRenderTargets())
		//{
		//	ERROR_MESSAGE_W(L"멀티 렌더타겟 생성 실패.");
		//	std::abort();
		//}

		LoadDefaultMesh();
		LoadDefaultShader();
		
		CreateSamplerStates();
		CreateRasterizerStates();
		CreateDepthStencilStates();
		CreateBlendStates();

		CreateBuffer();
		LoadDefaultTexture();
		LoadDefaultMaterial();
	}

	void RenderMgr::Release()
	{
		mMainCamera = nullptr;
		mInspectorGameObject = nullptr;
		for (int i = 0; i < (int)eCBType::END; ++i)
		{
			mConstBuffers->reset();
		}
		for (int i = 0; i < (int)eSamplerType::END; ++i)
		{
			mSamplerStates[i] = nullptr;
		}
		for (int i = 0; i < (int)eRSType::END; ++i)
		{
			mRasterizerStates[i] = nullptr;
		}
		for (int i = 0; i < (int)eDSType::END; ++i)
		{
			mDepthStencilStates[i] = nullptr;
		}
		for (int i = 0; i < (int)eBSType::END; ++i)
		{
			mBlendStates[i] = nullptr;
		}

		for (size_t i = 0; i < mCameras.size(); ++i)
		{
			SAFE_DELETE(mCameras[i]);
		}

		mDebugMeshes.clear();
		mLightAttributes.clear();
		mLightsBuffer.reset();
		mPostProcessTexture = nullptr;

		for (int i = 0; i < (int)eMRTType::END; ++i)
		{
			mMultiRenderTargets[i].reset();
		}
		
	}

	void RenderMgr::Render()
	{
		ClearMultiRenderTargets();

		UpdateGlobalCBuffer();

		BindNoiseTexture();
		BindLights();

		for (Com_Camera* cam : mCameras)
		{
			if (cam == nullptr)
				continue;

			cam->RenderCamera();
		}

		mCameras.clear();
		mLightAttributes.clear();
	}


	void RenderMgr::RemoveLight(Com_Light3D* _pComLight)
	{
		for (auto iter = mLights.begin(); iter != mLights.end(); ++iter)
		{
			if (*iter == _pComLight)
			{
				mLights.erase(iter);
				break;
			}
		}
	}

	void RenderMgr::BindLights()
	{
		mLightsBuffer->SetData(mLightAttributes.data(), mLightAttributes.size());

		eShaderStageFlag_ Flag = eShaderStageFlag::VS | eShaderStageFlag::PS;

		mLightsBuffer->BindDataSRV(Register_t_lightAttributes, Flag);

		
		tCB_NumberOfLight trCb = {};
		trCb.numberOfLight = (uint)mLightAttributes.size();

		for (size_t i = 0; i < mLights.size(); i++)
		{
			mLights[i]->SetIndex((uint)i);
		}

		ConstBuffer* cb = mConstBuffers[(uint)eCBType::numberOfLight].get();
		cb->SetData(&trCb);
		cb->BindData(Flag);
	}
	void RenderMgr::BindNoiseTexture()
	{
		std::shared_ptr<Texture> noise = ResMgr::Find<Texture>(define::strKey::Default::texture::noise_03);
		noise->BindDataSRV(Register_t_NoiseTexture, eShaderStageFlag::ALL);

		tCB_Noise info = {};
		info.NoiseSize.x = (float)noise->GetWidth();
		info.NoiseSize.y = (float)noise->GetHeight();

		static float noiseTime = 10.f;
		noiseTime -= TimeMgr::DeltaTime();
		info.NoiseTime = noiseTime;

		ConstBuffer* cb = mConstBuffers[(uint)eCBType::Noise].get();
		cb->SetData(&info);
		cb->BindData(eShaderStageFlag::ALL);
	}
	void RenderMgr::CopyRenderTarget()
	{
		std::shared_ptr<Texture> renderTarget = ResMgr::Find<Texture>(define::strKey::Default::texture::RenderTarget);
			
		//renderTarget->UnBindData();

		//ID3D11ShaderResourceView* srv = nullptr;
		//GPUMgr::Context()->PSSetShaderResources()(eShaderStage::PS, 60, &srv);

		ID3D11Texture2D* dest = mPostProcessTexture->GetTexture().Get();
		ID3D11Texture2D* source = renderTarget->GetTexture().Get();

		GPUMgr::Context()->CopyResource(dest, source);

		mPostProcessTexture->BindDataSRV(Register_t_postProcessTexture, eShaderStageFlag::PS);
	}

	void RenderMgr::ClearMultiRenderTargets()
	{
		for (int i = 0; i < (int)eMRTType::END; ++i)
		{
			if (mMultiRenderTargets[i])
			{
				if ((int)eMRTType::Swapchain == i)
				{
					mMultiRenderTargets[i]->Clear(float4(0.2f));
				}
				else
				{
					mMultiRenderTargets[i]->Clear(float4(0.f));
				}
			}
				
		}
	}

	void RenderMgr::UpdateGlobalCBuffer()
	{
		tCB_Global cb{};
		cb.uResolution.x = GPUMgr::GetResolutionX();
		cb.uResolution.y = GPUMgr::GetResolutionY();
		cb.fResolution.x = (float)cb.uResolution.x;
		cb.fResolution.y = (float)cb.uResolution.y;
		cb.DeltaTime = TimeMgr::DeltaTime();
		ConstBuffer* global = GetConstBuffer(eCBType::Global);
		global->SetData(&cb);
		global->BindData();
	}

	bool RenderMgr::SetResolution(UINT _ResolutionX, UINT _ResolutionY)
	{
		if (false == CreateMultiRenderTargets(_ResolutionX, _ResolutionY))
		{
			ERROR_MESSAGE_W(L"해상도 변경 실패");
			return false;
		}

		for (auto* iter : mCameras)
		{
			if (iter)
			{
				iter->CreateProjectionMatrix(_ResolutionX, _ResolutionY);
			}
		}


		return true;
	}

	bool RenderMgr::CreateMultiRenderTargets(UINT _ResolutionX, UINT _ResolutionY)
	{
		{
			//Swapchain MRT
			std::shared_ptr<Texture> arrRTTex[8] = {};
			std::shared_ptr<Texture> dsTex = nullptr;

			arrRTTex[0] = GPUMgr::GetRenderTargetTex();
			dsTex = GPUMgr::GetDepthStencilBufferTex();

			mMultiRenderTargets[(UINT)eMRTType::Swapchain] = std::make_unique<MultiRenderTarget>();

			if (false == mMultiRenderTargets[(UINT)eMRTType::Swapchain]->Create(arrRTTex, dsTex))
			{
				ERROR_MESSAGE_W(L"Multi Render Target 생성 실패.");
				return false;
			}
		}

		// Deffered MRT
		{
			std::shared_ptr<Texture> arrRTTex[MRT_MAX] = {};
			std::shared_ptr<Texture> dsTex = nullptr;

			
			//std::shared_ptr<Texture> normal = std::make_shared<Texture>();
			//std::shared_ptr<Texture> albedo = std::make_shared<Texture>();
			//std::shared_ptr<Texture> specular = std::make_shared<Texture>();
			//std::shared_ptr<Texture> emissive = std::make_shared<Texture>();

			//arrRTTex[(int)eMRT_Defferd::AlbedoTarget] = albedo;
			//arrRTTex[(int)eMRT_Defferd::NormalTarget] = normal;
			//arrRTTex[(int)eMRT_Defferd::SpecularTarget] = specular;
			//arrRTTex[(int)eMRT_Defferd::EmissiveTarget] = emissive;
			//arrRTTex[(int)eMRT_Defferd::PositionTarget] = pos;
			
			for (int i = 0; i < (int)eMRT_Defferd::END; ++i)
			{
				std::shared_ptr<Texture> defferedTex = std::make_shared<Texture>();
				arrRTTex[i] = defferedTex;
				arrRTTex[i]->Create(_ResolutionX, _ResolutionY, DXGI_FORMAT_R32G32B32A32_FLOAT
					, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			}

			dsTex = GPUMgr::GetDepthStencilBufferTex();

			mMultiRenderTargets[(int)eMRTType::Deffered] = std::make_unique<MultiRenderTarget>();
			if(false == mMultiRenderTargets[(int)eMRTType::Deffered]->Create(arrRTTex, dsTex))
			{
				ERROR_MESSAGE_W(L"Multi Render Target 생성 실패.");
				return false;
			}
		}


		// Light MRT
		{
			std::shared_ptr<Texture> arrRTTex[MRT_MAX] = { };

			for (int i = 0; i < (int)eMRT_Light::END; ++i)
			{
				std::shared_ptr<Texture> defferedTex = std::make_shared<Texture>();
				arrRTTex[i] = defferedTex;
				arrRTTex[i]->Create(_ResolutionX, _ResolutionY, DXGI_FORMAT_R32G32B32A32_FLOAT
					, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			}

			//std::shared_ptr<Texture> diffuse = std::make_shared<Texture>();
			//std::shared_ptr<Texture> specular = std::make_shared<Texture>();

			//arrRTTex[(int)eMRT_Light::DiffuseLightTarget] = diffuse;
			//arrRTTex[(int)eMRT_Light::SpecularLightTarget] = specular;

			//arrRTTex[0]->Create(_ResolutionX, _ResolutionY, DXGI_FORMAT_R32G32B32A32_FLOAT
			//	, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			//arrRTTex[1]->Create(_ResolutionX, _ResolutionY, DXGI_FORMAT_R32G32B32A32_FLOAT
			//	, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

			mMultiRenderTargets[(int)eMRTType::Light] = std::make_unique<MultiRenderTarget>();
			mMultiRenderTargets[(int)eMRTType::Light]->Create(arrRTTex, nullptr);
		}


		return true;
	}

	void RenderMgr::LoadDefaultMesh()
	{
		using namespace mh::define;
		

#pragma region POINT MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			Vertex2D vtx2d = {};
			std::shared_ptr<Mesh> pointMesh = std::make_shared<Mesh>();
			pointMesh->SetEngineDefaultRes(true);
			ResMgr::Insert(strKey::Default::mesh::PointMesh, pointMesh);

			pointMesh->CreateVertexBuffer(&vtx2d, sizeof(Vertex2D), (size_t)1);
			uint pointIndex = 0;
			pointMesh->CreateIndexBuffer(&pointIndex, 1u);
		}

#pragma endregion
#pragma region RECT MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			Vertex2D vtx2d = {};

			//RECT
			vtx2d.Pos = float4(-0.5f, 0.5f, 0.0f, 1.0f);
			vtx2d.UV = float2(0.f, 0.f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = float4(0.5f, 0.5f, 0.0f, 1.0f);
			vtx2d.UV = float2(1.0f, 0.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = float4(0.5f, -0.5f, 0.0f, 1.0f);
			vtx2d.UV = float2(1.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = float4(-0.5f, -0.5f, 0.0f, 1.0f);
			vtx2d.UV = float2(0.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			// Crate Mesh
			std::shared_ptr<Mesh> RectMesh = std::make_shared<Mesh>();
			RectMesh->SetEngineDefaultRes(true);
			ResMgr::Insert(strKey::Default::mesh::RectMesh, RectMesh);
			RectMesh->CreateVertexBuffer(VecVtx2D.data(), sizeof(vtx2d), VecVtx2D.size());


			std::vector<uint> indices = { 0u , 1u, 2u, 0u, 2u, 3u, 0u };
			RectMesh->CreateIndexBuffer(indices.data(), (uint)indices.size());

			VecVtx2D.clear();

#pragma endregion
#pragma region DEBUG RECTMESH

			vtx2d.Pos = float4(-0.5f, 0.5f, -0.00001f, 1.0f);
			vtx2d.UV = float2(0.f, 0.f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = float4(0.5f, 0.5f, -0.00001f, 1.0f);
			vtx2d.UV = float2(1.0f, 0.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = float4(0.5f, -0.5f, -0.00001f, 1.0f);
			vtx2d.UV = float2(1.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = float4(-0.5f, -0.5f, -0.00001f, 1.0f);
			vtx2d.UV = float2(0.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};
			
			// Create Mesh
			std::shared_ptr<Mesh> debugmesh = std::make_shared<Mesh>();
			ResMgr::Insert(strKey::Default::mesh::DebugRectMesh, debugmesh);
			debugmesh->CreateVertexBuffer(VecVtx2D.data(), sizeof(Vertex2D), VecVtx2D.size());
			debugmesh->CreateIndexBuffer(indices.data(), static_cast<uint>(indices.size()));
		}
#pragma endregion
#pragma region CIRCLE MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			std::vector<uint>     VecIdx;

			Vertex2D center{};
			center.Pos = float4(0.0f, 0.0f, 0.f, 1.0f);
			center.UV = float2(0.5f, 0.5f);
			VecVtx2D.push_back(center);

			int iSlice = 40;
			float fRadius = 0.5f;
			float fTheta = XM_2PI / (float)iSlice;

			Vertex2D vtx2d = {};
			for (int i = 0; i < iSlice; ++i)
			{
				vtx2d.Pos.x = fRadius * cosf(fTheta * i);
				vtx2d.Pos.y = fRadius * sinf(fTheta * i);

				//UV는 중점 기준으로 더하거나 빼는 방식으로 해준다.
				vtx2d.UV.x = 0.5f + vtx2d.Pos.x;
				vtx2d.UV.y = 0.5f - vtx2d.Pos.y;	//반대 방향

				VecVtx2D.push_back(vtx2d);

				//정점배열의 첫번째는 중심점이 들어가 있으므로 i + 1 == 현재 정점버퍼의 사이즈가 된다
				//인덱스는 시계 방향으로 삽입
				//마지막 인덱스는 따로 직접 삽입
				if (i == (iSlice - 1))
					continue;

				VecIdx.push_back(0);
				VecIdx.push_back(i + 2);
				VecIdx.push_back(i + 1);
			}

			VecIdx.push_back(0);
			VecIdx.push_back(1);
			VecIdx.push_back((uint)VecVtx2D.size() - 1u);

			// Crate Mesh
			std::shared_ptr<Mesh> cirlceMesh = std::make_shared<Mesh>();
			ResMgr::Insert(strKey::Default::mesh::CircleMesh, cirlceMesh);
			cirlceMesh->CreateVertexBuffer(VecVtx2D.data(), sizeof(Vertex2D), VecVtx2D.size());
			cirlceMesh->CreateIndexBuffer(VecIdx.data(), VecIdx.size());
		}
#pragma endregion
#pragma region Cube Mesh
		{
			Vertex3D vtx3d;
			std::vector<Vertex3D> VecVtx3D;
			VecVtx3D.reserve(24);

			
			// 윗면
			vtx3d.Pos = float4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(0.f, 1.f, 0.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(0.f, 1.f, 0.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(0.f, 1.f, 0.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(0.f, 1.f, 0.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(0.f, -1.f, 0.f);
			vtx3d.Tangent = float3(-1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(0.f, -1.f, 0.f);
			vtx3d.Tangent = float3(-1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(0.f, -1.f, 0.f);
			vtx3d.Tangent = float3(-1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(0.f, -1.f, 0.f);
			vtx3d.Tangent = float3(-1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(-1.f, 0.f, 0.f);
			vtx3d.Tangent = float3(0.0f, 1.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(-1.f, 0.f, 0.f);
			vtx3d.Tangent = float3(0.0f, 1.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(-1.f, 0.f, 0.f);
			vtx3d.Tangent = float3(0.0f, 1.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(-1.f, 0.f, 0.f);
			vtx3d.Tangent = float3(0.0f, 1.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(1.f, 0.f, 0.f);
			vtx3d.Tangent = float3(0.0f, -1.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(1.f, 0.f, 0.f);
			vtx3d.Tangent = float3(0.0f, -1.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(1.f, 0.f, 0.f);
			vtx3d.Tangent = float3(0.0f, -1.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(1.f, 0.f, 0.f);
			vtx3d.Tangent = float3(0.0f, -1.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(0.f, 0.f, 1.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(0.f, 0.f, 1.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(0.f, 0.f, 1.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(0.f, 0.f, 1.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, 0.5f, -0.5f, 1.0f);;
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(0.f, 0.f, -1.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(0.f, 0.f, -1.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(0.f, 0.f, -1.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = float4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(0.f, 0.f, -1.f);
			vtx3d.Tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = float3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			std::vector<uint> indices;
			for (size_t i = 0; i < 6; i++)
			{
				indices.push_back(static_cast<int>(i) * 4);
				indices.push_back(static_cast<int>(i) * 4 + 1);
				indices.push_back(static_cast<int>(i) * 4 + 2);

				indices.push_back(static_cast<int>(i) * 4);
				indices.push_back(static_cast<int>(i) * 4 + 2);
				indices.push_back(static_cast<int>(i) * 4 + 3);
			}

			// Crate Mesh
			std::shared_ptr<Mesh> cubMesh = std::make_shared<Mesh>();
			ResMgr::Insert(strKey::Default::mesh::CubeMesh, cubMesh);
			cubMesh->Create<Vertex3D>(VecVtx3D, indices);
		}

#pragma endregion
#pragma region Sphere Mesh
		{
			Vertex3D vtx3d;
			std::vector<Vertex3D> VecVtx3D;
			VecVtx3D.reserve(2000);

			float fRadius = 0.5f;

			// Top
			vtx3d.Pos = float4(0.0f, fRadius, 0.0f, 1.0f);
			vtx3d.UV = float2(0.5f, 0.f);
			vtx3d.Normal = float3(0.0f, fRadius, 0.0f);
			vtx3d.Normal.Normalize();
			vtx3d.Tangent = float3(1.f, 0.f, 0.f);
			vtx3d.BiNormal = float3(0.f, 0.f, 1.f);
			VecVtx3D.push_back(vtx3d);

			// Body
			uint iStackCount = 40; // 가로 분할 개수
			uint iSliceCount = 40; // 세로 분할 개수

			float fStackAngle = XM_PI / iStackCount;
			float fSliceAngle = XM_2PI / iSliceCount;

			float fUVXStep = 1.f / (float)iSliceCount;
			float fUVYStep = 1.f / (float)iStackCount;

			for (uint i = 1; i < iStackCount; ++i)
			{
				float phi = i * fStackAngle;

				for (uint j = 0; j <= iSliceCount; ++j)
				{
					float theta = j * fSliceAngle;

					vtx3d.Pos = float4(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
						, fRadius * cosf(i * fStackAngle)
						, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle), 1.0f);
					vtx3d.UV = float2(fUVXStep * j, fUVYStep * i);
					vtx3d.Normal = float3(vtx3d.Pos);
					//v.Normal.Normalize();

					vtx3d.Tangent.x = -fRadius * sinf(phi) * sinf(theta);
					vtx3d.Tangent.y = 0.f;
					vtx3d.Tangent.z = fRadius * sinf(phi) * cosf(theta);
					vtx3d.Tangent.Normalize();

					vtx3d.Tangent.Cross(vtx3d.Normal, vtx3d.BiNormal);
					vtx3d.BiNormal.Normalize();

					VecVtx3D.push_back(vtx3d);
					vtx3d = {};
				}
			}

			// Bottom
			vtx3d.Pos = float4(0.f, -fRadius, 0.f, 1.0f);
			vtx3d.UV = float2(0.5f, 1.f);
			vtx3d.Normal = float3(vtx3d.Pos.x, vtx3d.Pos.y, vtx3d.Pos.z);
			vtx3d.Normal.Normalize();

			vtx3d.Tangent = float3(1.f, 0.f, 0.f);
			vtx3d.BiNormal = float3(0.f, 0.f, -1.f);
			VecVtx3D.push_back(vtx3d);

			// 인덱스
			// 북극점
			std::vector<uint> indices;
			indices.reserve(10000);
			for (uint i = 0; i < iSliceCount; ++i)
			{
				indices.push_back(0);
				indices.push_back(i + 2);
				indices.push_back(i + 1);
			}

			// 몸통
			for (uint i = 0; i < iStackCount - 2; ++i)
			{
				for (uint j = 0; j < iSliceCount; ++j)
				{
					// + 
					// | \
					// +--+
					indices.push_back((iSliceCount + 1) * (i)+(j)+1);
					indices.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
					indices.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

					// +--+
					//  \ |
					//    +
					indices.push_back((iSliceCount + 1) * (i)+(j)+1);
					indices.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
					indices.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				}
			}

			// 남극점
			uint iBottomIdx = (uint)VecVtx3D.size() - 1;
			for (uint i = 0; i < iSliceCount; ++i)
			{
				indices.push_back(iBottomIdx);
				indices.push_back(iBottomIdx - (i + 2));
				indices.push_back(iBottomIdx - (i + 1));
			}

			std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>();
			ResMgr::Insert(strKey::Default::mesh::SphereMesh, sphereMesh);

			sphereMesh->Create<Vertex3D>(VecVtx3D, indices);
		}


#pragma endregion
	}

	void RenderMgr::LoadDefaultShader()
	{
#pragma region 2D LAYOUT
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc2D;
		D3D11_INPUT_ELEMENT_DESC LayoutDesc{};

		LayoutDesc.AlignedByteOffset = 0;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "POSITION";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc2D.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		LayoutDesc.AlignedByteOffset = 16;
		LayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TEXCOORD";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc2D.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
#pragma endregion


#pragma region DEFAULT TRIANGLE SHADER
		{
			std::shared_ptr<GraphicsShader> TriangleShader = std::make_shared<GraphicsShader>();
			TriangleShader->SetEngineDefaultRes(true);
			TriangleShader->CreateByHeader(eGSStage::VS, _0VS_Triangle, sizeof(_0VS_Triangle));
			TriangleShader->CreateByHeader(eGSStage::PS, _4PS_Triangle, sizeof(_4PS_Triangle));
			TriangleShader->SetInputLayoutDesc(vecLayoutDesc2D);
			TriangleShader->CreateInputLayout();

			ResMgr::Insert(strKey::Default::shader::graphics::RectShader, TriangleShader);
		}
#pragma endregion
#pragma region SPRITE SHADER
		{
			std::shared_ptr<GraphicsShader> spriteShader = std::make_shared<GraphicsShader>();
			spriteShader->SetEngineDefaultRes(true);
			spriteShader->CreateByHeader(eGSStage::VS, _0VS_Sprite, sizeof(_0VS_Sprite));
			spriteShader->CreateByHeader(eGSStage::PS, _4PS_Sprite, sizeof(_4PS_Sprite));
			spriteShader->SetRSState(eRSType::SolidNone);
			spriteShader->SetInputLayoutDesc(vecLayoutDesc2D);
			spriteShader->CreateInputLayout();


			ResMgr::Insert(strKey::Default::shader::graphics::SpriteShader, spriteShader);
		}

#pragma endregion
#pragma region UI SHADER
		{
			std::shared_ptr<GraphicsShader> uiShader = std::make_shared<GraphicsShader>();
			uiShader->SetEngineDefaultRes(true);
			uiShader->CreateByHeader(eGSStage::VS, _0VS_UserInterface, sizeof(_0VS_UserInterface));
			uiShader->CreateByHeader(eGSStage::PS, _4PS_UserInterface, sizeof(_4PS_UserInterface));
			uiShader->SetInputLayoutDesc(vecLayoutDesc2D);
			uiShader->CreateInputLayout();


			ResMgr::Insert(strKey::Default::shader::graphics::UIShader, uiShader);
		}

#pragma endregion
#pragma region GRID SHADER
		{
			std::shared_ptr<GraphicsShader> gridShader = std::make_shared<GraphicsShader>();
			gridShader->SetEngineDefaultRes(true);
			gridShader->CreateByHeader(eGSStage::VS, _0VS_Grid, sizeof(_0VS_Grid));
			gridShader->CreateByHeader(eGSStage::PS, _4PS_Grid, sizeof(_4PS_Grid));
			gridShader->SetInputLayoutDesc(vecLayoutDesc2D);
			gridShader->CreateInputLayout();

			gridShader->SetRSState(eRSType::SolidNone);
			gridShader->SetDSState(eDSType::NoWrite);
			gridShader->SetBSState(eBSType::AlphaBlend);

			ResMgr::Insert(strKey::Default::shader::graphics::GridShader, gridShader);
		}

#pragma endregion
#pragma region DEBUG SHADER
		{
			std::shared_ptr<GraphicsShader> debugShader = std::make_shared<GraphicsShader>();
			debugShader->SetEngineDefaultRes(true);
			debugShader->CreateByHeader(eGSStage::VS, _0VS_Debug, sizeof(_0VS_Debug));
			debugShader->CreateByHeader(eGSStage::PS, _4PS_Debug, sizeof(_4PS_Debug));
			debugShader->SetInputLayoutDesc(vecLayoutDesc2D);
			debugShader->CreateInputLayout();

			//debugShader->Create(eShaderStage::VS, "DebugVS.hlsl", "main");
			//debugShader->Create(eShaderStage::PS, "DebugPS.hlsl", "main");
			debugShader->SetRSState(eRSType::SolidNone);
			debugShader->SetDSState(eDSType::NoWrite);
			debugShader->SetBSState(eBSType::AlphaBlend);
			debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

			ResMgr::Insert(strKey::Default::shader::graphics::DebugShader, debugShader);
		}

#pragma endregion
#pragma region PAINT SHADER
		{
			std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
			paintShader->SetEngineDefaultRes(true);
			paintShader->CreateByHeader(CS_Paint, sizeof(CS_Paint));
			ResMgr::Insert(strKey::Default::shader::graphics::PaintShader, paintShader);
		}

#pragma endregion
#pragma region PARTICLE SHADER
		{
			std::shared_ptr<GraphicsShader> particleShader = std::make_shared<GraphicsShader>();
			particleShader->SetEngineDefaultRes(true);

			particleShader->CreateByHeader(eGSStage::VS, _0VS_Particle, sizeof(_0VS_Particle));
			particleShader->CreateByHeader(eGSStage::GS, _3GS_Particle, sizeof(_3GS_Particle));
			particleShader->CreateByHeader(eGSStage::PS, _4PS_Particle, sizeof(_4PS_Particle));
			particleShader->SetInputLayoutDesc(vecLayoutDesc2D);
			particleShader->CreateInputLayout();


			particleShader->SetRSState(eRSType::SolidNone);
			particleShader->SetDSState(eDSType::NoWrite);
			particleShader->SetBSState(eBSType::AlphaBlend);
			particleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			ResMgr::Insert(strKey::Default::shader::graphics::ParticleShader, particleShader);

			std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
			particleCS->SetEngineDefaultRes(true);
			ResMgr::Insert(strKey::Default::shader::compute::ParticleCS, particleCS);
			particleCS->CreateByHeader(CS_Particle, sizeof(CS_Particle));
		}
#pragma endregion
#pragma region POST PROCESS SHADER
		{
			std::shared_ptr<GraphicsShader> postProcessShader = std::make_shared<GraphicsShader>();
			postProcessShader->SetEngineDefaultRes(true);
			postProcessShader->CreateByHeader(eGSStage::VS, _0VS_PostProcess, sizeof(_0VS_PostProcess));
			postProcessShader->CreateByHeader(eGSStage::PS, _4PS_PostProcess, sizeof(_4PS_PostProcess));
			postProcessShader->SetInputLayoutDesc(vecLayoutDesc2D);
			postProcessShader->CreateInputLayout();

			postProcessShader->SetDSState(eDSType::NoWrite);
			ResMgr::Insert(strKey::Default::shader::graphics::PostProcessShader, postProcessShader);
		}
#pragma endregion
//#pragma region SKY BOX SHADER
//		{
//			std::shared_ptr<GraphicsShader> skyBoxShader = std::make_shared<GraphicsShader>();
//			skyBoxShader->SetEngineDefaultRes(true);
//			skyBoxShader->CreateByHeader(eGSStage::VS, _0VS_SkyBox, sizeof(_0VS_SkyBox));
//			skyBoxShader->CreateByHeader(eGSStage::PS, _4PS_SkyBox, sizeof(_4PS_SkyBox));
//			skyBoxShader->SetInputLayoutDesc(vecLayoutDesc2D);
//			skyBoxShader->CreateInputLayout();
//
//			skyBoxShader->SetRSState(eRSType::SolidFront);
//			skyBoxShader->SetDSState(eDSType::Less);
//			skyBoxShader->SetBSState(eBSType::Default);
//			ResMgr::Insert(strKey::Default::shader::graphics::SkyBoxShader, skyBoxShader);
//		}
//#pragma endregion

#pragma region 3D 기본 입력 레이아웃
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc3D = vecLayoutDesc2D;

		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 24;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TANGENT";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);
		
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 36;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BINORMAL";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);
		
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 48;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "NORMAL";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 60;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BLENDWEIGHT";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 76;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BLENDINDICES";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);


#pragma endregion

#pragma region BASIC 3D
		{
			std::shared_ptr<GraphicsShader> basic3DShader = std::make_shared<GraphicsShader>();
			basic3DShader->SetEngineDefaultRes(true);

			basic3DShader->CreateByHeader(eGSStage::VS, _0VS_Basic3D, sizeof(_0VS_Basic3D));
			basic3DShader->CreateByHeader(eGSStage::PS, _4PS_Basic3D, sizeof(_4PS_Basic3D));
			basic3DShader->SetInputLayoutDesc(vecLayoutDesc3D);
			basic3DShader->CreateInputLayout();

			ResMgr::Insert(strKey::Default::shader::graphics::Basic3DShader, basic3DShader);
		}
#pragma endregion

#pragma region DEFFERD
		std::shared_ptr<GraphicsShader> defferedShader = std::make_shared<GraphicsShader>();
		defferedShader->SetEngineDefaultRes(true);

		defferedShader->CreateByHeader(eGSStage::VS, _0VS_Deffered, sizeof(_0VS_Deffered));
		defferedShader->CreateByHeader(eGSStage::PS, _4PS_Deffered, sizeof(_4PS_Deffered));

		defferedShader->SetInputLayoutDesc(vecLayoutDesc3D);
		defferedShader->CreateInputLayout();

		ResMgr::Insert(strKey::Default::shader::graphics::DefferedShader, defferedShader);

		//defferdShader->SetRSState();
#pragma endregion


#pragma region LIGHT
		{
			std::shared_ptr<GraphicsShader> lightShader = std::make_shared<GraphicsShader>();
			lightShader->CreateByHeader(eGSStage::VS, _0VS_LightDir, sizeof(_0VS_LightDir));
			lightShader->CreateByHeader(eGSStage::PS, _4PS_LightDir, sizeof(_4PS_LightDir));

			lightShader->SetInputLayoutDesc(vecLayoutDesc2D);
			lightShader->CreateInputLayout();

			lightShader->SetRSState(eRSType::SolidBack);
			lightShader->SetDSState(eDSType::None);
			lightShader->SetBSState(eBSType::OneOne);

			ResMgr::Insert(strKey::Default::shader::graphics::LightDirShader, lightShader);
		}

		{
			std::shared_ptr<GraphicsShader> lightShader = std::make_shared<GraphicsShader>();
			lightShader->CreateByHeader(eGSStage::VS, _0VS_LightPoint, sizeof(_0VS_LightPoint));
			lightShader->CreateByHeader(eGSStage::PS, _4PS_LightPoint, sizeof(_4PS_LightPoint));

			lightShader->SetInputLayoutDesc(vecLayoutDesc3D);
			lightShader->CreateInputLayout();

			lightShader->SetRSState(eRSType::SolidFront);
			lightShader->SetDSState(eDSType::None);
			lightShader->SetBSState(eBSType::OneOne);

			ResMgr::Insert(strKey::Default::shader::graphics::LightPointShader, lightShader);
		}
#pragma endregion



#pragma region MERGE
		{
			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
			LayoutDesc.AlignedByteOffset = 0;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "POSITION";
			LayoutDesc.SemanticIndex = 0;

			std::shared_ptr<GraphicsShader> MergeShader = std::make_shared<GraphicsShader>();
			MergeShader->SetEngineDefaultRes(true);
			MergeShader->CreateByHeader(eGSStage::VS, _0VS_Merge, sizeof(_0VS_Merge));
			MergeShader->CreateByHeader(eGSStage::PS, _4PS_Merge, sizeof(_4PS_Merge));

			MergeShader->AddInputLayoutDesc(LayoutDesc);
			MergeShader->CreateInputLayout();

			MergeShader->SetRSState(eRSType::SolidBack);
			MergeShader->SetDSState(eDSType::None);
			MergeShader->SetBSState(eBSType::Default);

			ResMgr::Insert(strKey::Default::shader::graphics::MergeShader, MergeShader);
		}

#pragma endregion


#pragma region ANIMATION 3D
		std::shared_ptr<Animation3DShader> Anim3DShader = std::make_shared<Animation3DShader>();
		Anim3DShader->SetEngineDefaultRes(true);
		Anim3DShader->CreateByHeader(CS_Animation3D, sizeof(CS_Animation3D));

		ResMgr::Insert(strKey::Default::shader::compute::Animation3D, Anim3DShader);
#pragma endregion
	}

	void RenderMgr::CreateBuffer()
	{
#pragma region CONSTANT BUFFER
		mConstBuffers[(uint)eCBType::Global] = std::make_unique<ConstBuffer>(eCBType::Global);
		mConstBuffers[(uint)eCBType::Global]->Create(sizeof(tCB_Global));
		mConstBuffers[(uint)eCBType::Global]->SetPresetTargetStage(eShaderStageFlag::ALL);

		UpdateGlobalCBuffer();

		mConstBuffers[(uint)eCBType::Transform] = std::make_unique<ConstBuffer>(eCBType::Transform);
		mConstBuffers[(uint)eCBType::Transform]->Create(sizeof(tCB_Transform));

		mConstBuffers[(uint)eCBType::ComputeShader] = std::make_unique<ConstBuffer>(eCBType::ComputeShader);
		mConstBuffers[(uint)eCBType::ComputeShader]->Create<tCB_ComputeShader>(1u);
		mConstBuffers[(uint)eCBType::ComputeShader]->SetPresetTargetStage(eShaderStageFlag::CS);

		mConstBuffers[(uint)eCBType::Material] = std::make_unique<ConstBuffer>(eCBType::Material);
		mConstBuffers[(uint)eCBType::Material]->Create(sizeof(tCB_MaterialData));

		mConstBuffers[(uint)eCBType::Grid] = std::make_unique<ConstBuffer>(eCBType::Grid);
		mConstBuffers[(uint)eCBType::Grid]->Create(sizeof(tCB_Grid));

		mConstBuffers[(uint)eCBType::Animation2D] = std::make_unique<ConstBuffer>(eCBType::Animation2D);
		mConstBuffers[(uint)eCBType::Animation2D]->Create(sizeof(tCB_Animation2D));

		mConstBuffers[(uint)eCBType::numberOfLight] = std::make_unique<ConstBuffer>(eCBType::numberOfLight);
		mConstBuffers[(uint)eCBType::numberOfLight]->Create(sizeof(tCB_NumberOfLight));

		mConstBuffers[(uint)eCBType::ParticleSystem] = std::make_unique<ConstBuffer>(eCBType::ParticleSystem);
		mConstBuffers[(uint)eCBType::ParticleSystem]->Create(sizeof(tCB_ParticleSystem));

		mConstBuffers[(uint)eCBType::Noise] = std::make_unique<ConstBuffer>(eCBType::Noise);
		mConstBuffers[(uint)eCBType::Noise]->Create(sizeof(tCB_Noise));

		mConstBuffers[(uint)eCBType::SBufferCount] = std::make_unique<ConstBuffer>(eCBType::SBufferCount);
		mConstBuffers[(uint)eCBType::SBufferCount]->Create<tCB_SBufferCount>();

		mConstBuffers[(uint)eCBType::Animation3D] = std::make_unique<ConstBuffer>(eCBType::Animation3D);
		mConstBuffers[(uint)eCBType::Animation3D]->Create<tCB_Animation3D>();

		mConstBuffers[(uint)eCBType::UniformData] = std::make_unique<ConstBuffer>(eCBType::UniformData);
		mConstBuffers[(uint)eCBType::UniformData]->Create<tCB_UniformData>();

#pragma endregion
#pragma region STRUCTED BUFFER
		tSBufferDesc SDesc{};
		SDesc.eSBufferType = eStructBufferType::READ_ONLY;
		mLightsBuffer = std::make_unique<StructBuffer>(SDesc);
		mLightsBuffer->Create<tLightAttribute>(128u, nullptr, 0);
#pragma endregion
	}



	void RenderMgr::LoadDefaultTexture()
	{
#pragma region STATIC TEXTURE
		using namespace strKey::Default;

		std::shared_ptr<Texture> pTex = nullptr;
		pTex = ResMgr::Load<Texture>(texture::DefaultSprite);
		assert(nullptr != pTex);

		pTex = ResMgr::Load<Texture>(texture::CartoonSmoke);
		assert(nullptr != pTex);

		pTex = ResMgr::Load<Texture>(texture::noise_01);
		assert(nullptr != pTex);

		pTex = ResMgr::Load<Texture>(texture::noise_02);
		assert(nullptr != pTex);

		pTex = ResMgr::Load<Texture>(texture::noise_03);
		assert(nullptr != pTex);

		pTex = ResMgr::Load<Texture>(texture::BasicCube);
		assert(nullptr != pTex);

		pTex = ResMgr::Load<Texture>(texture::BasicCubeNormal);
		assert(nullptr != pTex);

		pTex = ResMgr::Load<Texture>(texture::Brick);
		assert(nullptr != pTex);

		pTex = ResMgr::Load<Texture>(texture::Brick_N);
		assert(nullptr != pTex);

		//pTex = ResMgr::Load<Texture>(texture::SkyBox_Water);
		//assert(nullptr != pTex);

#pragma endregion
#pragma region DYNAMIC TEXTURE
		std::shared_ptr<Texture> uavTexture = std::make_shared<Texture>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
			| D3D11_BIND_UNORDERED_ACCESS);
		ResMgr::Insert(texture::PaintTexture, uavTexture);
#pragma endregion

		//noise
		std::shared_ptr<Texture> mNoiseTex = std::make_shared<Texture>();
		mNoiseTex->Create(GPUMgr::GetResolutionX(), GPUMgr::GetResolutionY(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		mNoiseTex->BindDataSRV(Register_t_NoiseTexture, eShaderStageFlag::PS);
	}


	void RenderMgr::CreateSamplerStates()
	{

#pragma region sampler state
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;


		GPUMgr::Device()->CreateSamplerState
		(
			&samplerDesc
			, mSamplerStates[(uint)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		GPUMgr::Device()->CreateSamplerState
		(
			&samplerDesc
			, mSamplerStates[(uint)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		GPUMgr::Device()->CreateSamplerState
		(
			&samplerDesc
			, mSamplerStates[(uint)eSamplerType::Anisotropic].GetAddressOf()
		);


		GPUMgr::Context()->PSSetSamplers((uint)eSamplerType::Point
			, 1, mSamplerStates[(uint)eSamplerType::Point].GetAddressOf());

		GPUMgr::Context()->PSSetSamplers((uint)eSamplerType::Linear
			, 1, mSamplerStates[(uint)eSamplerType::Linear].GetAddressOf());

		GPUMgr::Context()->PSSetSamplers((uint)eSamplerType::Anisotropic
			, 1, mSamplerStates[(uint)eSamplerType::Anisotropic].GetAddressOf());

#pragma endregion
	}

	void RenderMgr::CreateRasterizerStates()
	{
#pragma region Rasterizer state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rsDesc.DepthClipEnable = TRUE;

		GPUMgr::Device()->CreateRasterizerState(&rsDesc
			, mRasterizerStates[(uint)eRSType::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		rsDesc.DepthClipEnable = TRUE;

		GPUMgr::Device()->CreateRasterizerState(&rsDesc
			, mRasterizerStates[(uint)eRSType::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GPUMgr::Device()->CreateRasterizerState(&rsDesc
			, mRasterizerStates[(uint)eRSType::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GPUMgr::Device()->CreateRasterizerState(&rsDesc
			, mRasterizerStates[(uint)eRSType::WireframeNone].GetAddressOf());
#pragma endregion
	}

	void RenderMgr::CreateBlendStates()
	{

#pragma region Blend State
		//None
		mBlendStates[(uint)eBSType::Default] = nullptr;

		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GPUMgr::Device()->CreateBlendState(&bsDesc, mBlendStates[(uint)eBSType::AlphaBlend].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GPUMgr::Device()->CreateBlendState(&bsDesc, mBlendStates[(uint)eBSType::OneOne].GetAddressOf());

#pragma endregion
	}

	void RenderMgr::CreateDepthStencilStates()
	{
#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		GPUMgr::Device()->CreateDepthStencilState(&dsDesc
			, mDepthStencilStates[(uint)eDSType::Less].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GPUMgr::Device()->CreateDepthStencilState(&dsDesc
			, mDepthStencilStates[(uint)eDSType::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GPUMgr::Device()->CreateDepthStencilState(&dsDesc
			, mDepthStencilStates[(uint)eDSType::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GPUMgr::Device()->CreateDepthStencilState(&dsDesc
			, mDepthStencilStates[(uint)eDSType::None].GetAddressOf());
#pragma endregion
	}


	void RenderMgr::LoadDefaultMaterial()
	{
		using namespace strKey::Default;
#pragma region DEFAULT
		std::shared_ptr <Texture> texture = ResMgr::Find<Texture>(texture::PaintTexture);
		texture->SetEngineDefaultRes(true);
		std::shared_ptr<GraphicsShader> shader = ResMgr::Find<GraphicsShader>(shader::graphics::RectShader);
		std::shared_ptr<Material> RectMaterial = std::make_shared<Material>();
		RectMaterial->SetShader(shader);
		RectMaterial->SetTexture(eTextureSlot::Albedo, texture);
		ResMgr::Insert(material::RectMaterial, RectMaterial);
#pragma endregion
#pragma region SPRITE
		std::shared_ptr <Texture> spriteTexture = ResMgr::Find<Texture>(texture::DefaultSprite);
		std::shared_ptr<GraphicsShader> spriteShader = ResMgr::Find<GraphicsShader>(shader::graphics::SpriteShader);
		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->SetRenderingMode(eRenderingMode::Opaque);
		spriteMaterial->SetShader(spriteShader);
		spriteMaterial->SetTexture(eTextureSlot::Albedo, spriteTexture);
		ResMgr::Insert(material::SpriteMaterial, spriteMaterial);
#pragma endregion
#pragma region UI
		std::shared_ptr<GraphicsShader> uiShader = ResMgr::Find<GraphicsShader>(shader::graphics::UIShader);
		std::shared_ptr<Material> uiMaterial = std::make_shared<Material>();
		uiMaterial->SetRenderingMode(eRenderingMode::Opaque);
		uiMaterial->SetShader(uiShader);
		ResMgr::Insert(material::UIMaterial, uiMaterial);
#pragma endregion
#pragma region GRID
		std::shared_ptr<GraphicsShader> gridShader = ResMgr::Find<GraphicsShader>(shader::graphics::GridShader);
		std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
		gridMaterial->SetShader(gridShader);
		ResMgr::Insert(material::GridMaterial, gridMaterial);
#pragma endregion
#pragma region DEBUG
		std::shared_ptr<GraphicsShader> debugShader = ResMgr::Find<GraphicsShader>(shader::graphics::DebugShader);
		std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
		debugMaterial->SetRenderingMode(eRenderingMode::Transparent);
		debugMaterial->SetShader(debugShader);
		ResMgr::Insert(material::DebugMaterial, debugMaterial);
#pragma endregion
#pragma region PARTICLE
		std::shared_ptr<GraphicsShader> particleShader = ResMgr::Find<GraphicsShader>(shader::graphics::ParticleShader);
		std::shared_ptr<Material> particleMaterial = std::make_shared<Material>();
		particleMaterial->SetRenderingMode(eRenderingMode::Transparent);
		particleMaterial->SetShader(particleShader);
		ResMgr::Insert(material::ParticleMaterial, particleMaterial);
#pragma endregion
#pragma region POSTPROCESS
		std::shared_ptr<GraphicsShader> postProcessShader = ResMgr::Find<GraphicsShader>(shader::graphics::PostProcessShader);
		std::shared_ptr<Material> postProcessMaterial = std::make_shared<Material>();
		postProcessMaterial->SetRenderingMode(eRenderingMode::PostProcess);
		postProcessMaterial->SetShader(postProcessShader);
		ResMgr::Insert(material::PostProcessMaterial, postProcessMaterial);
#pragma endregion

#pragma region BASIC3D
		std::shared_ptr<GraphicsShader> basic3DShader = ResMgr::Find<GraphicsShader>(shader::graphics::Basic3DShader);
		std::shared_ptr<Material> basic3DMaterial = std::make_shared<Material>();
		basic3DMaterial->SetRenderingMode(eRenderingMode::Transparent);
		basic3DMaterial->SetShader(basic3DShader);

		std::shared_ptr <Texture> albedo = ResMgr::Find<Texture>(texture::Brick);
		basic3DMaterial->SetTexture(eTextureSlot::Albedo, albedo);
		albedo = ResMgr::Find<Texture>(texture::Brick_N);
		basic3DMaterial->SetTexture(eTextureSlot::Normal, albedo);
		ResMgr::Insert(material::Basic3DMaterial, basic3DMaterial);
#pragma endregion

#pragma region DEFFERD
		std::shared_ptr<GraphicsShader> defferdShader = ResMgr::Find<GraphicsShader>(strKey::Default::shader::graphics::DefferedShader);

		std::shared_ptr<Material> defferdMaterial = std::make_shared<Material>();
		defferdMaterial->SetRenderingMode(eRenderingMode::DefferdOpaque);
		defferdMaterial->SetShader(defferdShader);

		// specular map 추가 사용가능
		albedo = ResMgr::Find<Texture>(strKey::Default::texture::Brick);
		defferdMaterial->SetTexture(eTextureSlot::Albedo, albedo);
		albedo = ResMgr::Find<Texture>(strKey::Default::texture::Brick_N);
		defferdMaterial->SetTexture(eTextureSlot::Normal, albedo);
		ResMgr::Insert(strKey::Default::material::DefferedMaterial, defferdMaterial);
#pragma endregion

#pragma region LIGHT
		{
			std::shared_ptr<GraphicsShader> lightShader = ResMgr::Find<GraphicsShader>(strKey::Default::shader::graphics::LightDirShader);
			std::shared_ptr<Material> lightMaterial = std::make_shared<Material>();
			lightMaterial->SetRenderingMode(eRenderingMode::None);
			lightMaterial->SetShader(lightShader);

			MultiRenderTarget* DefferedMRT = mMultiRenderTargets[(uint)eMRTType::Deffered].get();
			{
				std::shared_ptr<Texture> PosRenderTarget = DefferedMRT->GetRenderTarget((uint)eMRT_Defferd::PositionTarget);
				lightMaterial->SetTexture(eTextureSlot::PositionTarget, PosRenderTarget);
			}

			{
				std::shared_ptr<Texture> NormalRenderTarget = DefferedMRT->GetRenderTarget((uint)eMRT_Defferd::NormalTarget);
				lightMaterial->SetTexture(eTextureSlot::NormalTarget, NormalRenderTarget);
			}

			{
				std::shared_ptr<Texture> SpecularTarget = DefferedMRT->GetRenderTarget((uint)eMRT_Defferd::SpecularTarget);
				lightMaterial->SetTexture(eTextureSlot::SpecularTarget, SpecularTarget);
			}
			ResMgr::Insert(strKey::Default::material::LightDirMaterial, lightMaterial);
		}

		{
			std::shared_ptr<GraphicsShader> LightPointShader = ResMgr::Find<GraphicsShader>(strKey::Default::shader::graphics::LightPointShader);
			std::shared_ptr<Material> lightMaterial = std::make_shared<Material>();
			lightMaterial->SetRenderingMode(eRenderingMode::None);
			lightMaterial->SetShader(LightPointShader);

			MultiRenderTarget* DefferedMRT = mMultiRenderTargets[(uint)eMRTType::Deffered].get();
			{
				std::shared_ptr<Texture> PositionTarget = DefferedMRT->GetRenderTarget((uint)eMRT_Defferd::PositionTarget);
				lightMaterial->SetTexture(eTextureSlot::PositionTarget, PositionTarget);
			}
			
			{
				std::shared_ptr<Texture> NormalTarget = DefferedMRT->GetRenderTarget((uint)eMRT_Defferd::NormalTarget);
				lightMaterial->SetTexture(eTextureSlot::NormalTarget, NormalTarget);
			}

			{
				std::shared_ptr<Texture> SpecularTarget = DefferedMRT->GetRenderTarget((uint)eMRT_Defferd::SpecularTarget);
				lightMaterial->SetTexture(eTextureSlot::SpecularTarget, SpecularTarget);
			}

			ResMgr::Insert(strKey::Default::material::LightPointMaterial, lightMaterial);
		}

#pragma endregion
#pragma endregion

#pragma region MERGE
		std::shared_ptr<GraphicsShader> mergeShader = ResMgr::Find<GraphicsShader>(strKey::Default::shader::graphics::MergeShader);
		std::shared_ptr<Material> mergeMaterial = std::make_shared<Material>();
		mergeMaterial->SetRenderingMode(eRenderingMode::None);
		mergeMaterial->SetShader(mergeShader);

		{
			MultiRenderTarget* DefferedMRT = mMultiRenderTargets[(uint)eMRTType::Deffered].get();
			{
				std::shared_ptr<Texture> PosRenderTarget = DefferedMRT->GetRenderTarget((uint)eMRT_Defferd::PositionTarget);
				mergeMaterial->SetTexture(eTextureSlot::PositionTarget, PosRenderTarget);
			}

			{
				std::shared_ptr<Texture> AlbedoRenderTarget = DefferedMRT->GetRenderTarget((uint)eMRT_Defferd::AlbedoTarget);
				mergeMaterial->SetTexture(eTextureSlot::AlbedoTarget, AlbedoRenderTarget);
			}
		}

		{
			MultiRenderTarget* LightMRT = mMultiRenderTargets[(uint)eMRTType::Light].get();
			{
				std::shared_ptr<Texture> DiffuseLightTarget = LightMRT->GetRenderTarget((uint)eMRT_Light::DiffuseLightTarget);
				mergeMaterial->SetTexture(eTextureSlot::DiffuseLightTarget, DiffuseLightTarget);
			}
			{
				std::shared_ptr<Texture> SpecularLightTarget = LightMRT->GetRenderTarget((uint)eMRT_Light::SpecularLightTarget);
				mergeMaterial->SetTexture(eTextureSlot::SpecularLightTarget, SpecularLightTarget);
			}
		}


		ResMgr::Insert(strKey::Default::material::MergeMaterial, mergeMaterial);
#pragma endregion
//#pragma region SkyBox
//		std::shared_ptr<GraphicsShader> skyBoxShader = ResMgr::Find<GraphicsShader>(strKey::Default::shader::graphics::SkyBoxShader);
//		std::shared_ptr<Material> skyBoxMaterial = std::make_shared<Material>();
//		skyBoxMaterial->SetRenderingMode(eRenderingMode::Opaque);
//		skyBoxMaterial->SetShader(skyBoxShader);
//		
//		std::shared_ptr<Texture> skyBoxTexture = ResMgr::Find<Texture>(strKey::Default::texture::SkyBox_Water);
//		skyBoxMaterial->SetTexture(eTextureSlot::SkyBoxCube, skyBoxTexture);
//		
//		ResMgr::Insert(strKey::Default::material::SkyBoxMaterial, skyBoxMaterial);
//#pragma endregion
	}



}

