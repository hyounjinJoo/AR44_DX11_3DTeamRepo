#include "EnginePCH.h"
#include "RenderMgr.h"

#include "Mesh.h"
#include "Material.h"
#include "ResMgr.h"

#include "ConstBuffer.h"
#include "DefaultShaders.h"
#include "PaintShader.h"
#include "ParticleShader.h"

#include "SceneManager.h"

#include "ResMgr.h"
#include "TimeMgr.h"

namespace mh
{
	RenderMgr::RenderMgr()
		: mMainCamera{}
		, mConstBuffers{}
		, mSamplerStates{}
		, mRasterizerStates{}
		, mBlendStates{}
		, mCameras{}
		, mDebugMeshes{}
		, mLights{}
		, mLightsBuffer{}
		, mPostProcessTexture{}
		, mInspectorGameObject{}
	{

	}

	RenderMgr::~RenderMgr()
	{
		for (int i = 0; i < (int)eCBType::End; ++i)
		{
			if (nullptr == mConstBuffers[i])
				continue;

			delete mConstBuffers[i];
		}
	}
	void RenderMgr::Initialize()
	{
		LoadMesh();
		LoadShader();
		SetupState();
		LoadBuffer();
		LoadTexture();
		LoadMaterial();
	}
	void RenderMgr::Render()
	{
		BindNoiseTexture();
		BindLights();

		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		for (Com_Camera* cam : mCameras[(UINT)type])
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		mCameras[(UINT)type].clear();
		mLights.clear();
	}


	void RenderMgr::BindLights()
	{
		mLightsBuffer->SetData(mLights.data(), mLights.size());
		mLightsBuffer->BindSRV(eShaderStage::VS, 13);
		mLightsBuffer->BindSRV(eShaderStage::PS, 13);

		LightCB trCb = {};
		trCb.NumberOfLight = (UINT)mLights.size();

		ConstBuffer* cb = mConstBuffers[(UINT)eCBType::Light];
		cb->SetData(&trCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}
	void RenderMgr::BindNoiseTexture()
	{
		std::shared_ptr<Texture> noise = ResMgr::GetInst()->Find<Texture>(define::strKey::Default::texture::noise_03);
		noise->BindShaderResource(eShaderStage::VS, 16);
		noise->BindShaderResource(eShaderStage::HS, 16);
		noise->BindShaderResource(eShaderStage::DS, 16);
		noise->BindShaderResource(eShaderStage::GS, 16);
		noise->BindShaderResource(eShaderStage::PS, 16);
		noise->BindShaderResource(eShaderStage::CS, 16);

		NoiseCB info = {};
		info.NoiseSize.x = (float)noise->GetWidth();
		info.NoiseSize.y = (float)noise->GetHeight();

		static float noiseTime = 10.f;
		noiseTime -= TimeMgr::DeltaTime();
		info.NoiseTime = noiseTime;

		ConstBuffer* cb = mConstBuffers[(UINT)eCBType::Noise];
		cb->SetData(&info);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}
	void RenderMgr::CopyRenderTarget()
	{
		std::shared_ptr<Texture> renderTarget = ResMgr::GetInst()->Find<Texture>(define::strKey::Default::texture::RenderTarget);

		ID3D11ShaderResourceView* srv = nullptr;
		GPUMgr::GetInst()->BindShaderResource(eShaderStage::PS, 60, &srv);

		ID3D11Texture2D* dest = mPostProcessTexture->GetTexture().Get();
		ID3D11Texture2D* source = renderTarget->GetTexture().Get();

		GPUMgr::GetInst()->CopyResource(dest, source);

		mPostProcessTexture->BindShaderResource(eShaderStage::PS, 60);
	}
	void RenderMgr::LoadMesh()
	{
		using namespace mh::define;
		

#pragma region POINT MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			Vertex2D vtx2d = {};
			std::shared_ptr<Mesh> pointMesh = std::make_shared<Mesh>();
			pointMesh->SetEngineDefaultRes(true);
			ResMgr::GetInst()->Add(strKey::Default::mesh::PointMesh, pointMesh);

			pointMesh->CreateVertexBuffer(&vtx2d, sizeof(Vertex2D), (size_t)1);
			UINT pointIndex = 0;
			pointMesh->CreateIndexBuffer(&pointIndex, 1u);
		}

#pragma endregion
#pragma region RECT MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			Vertex2D vtx2d = {};

			//RECT
			vtx2d.Pos = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
			vtx2d.Color = Vector4(0.f, 1.f, 0.f, 1.f);
			vtx2d.UV = Vector2(0.f, 0.f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
			vtx2d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			vtx2d.UV = Vector2(1.0f, 0.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
			vtx2d.Color = Vector4(1.f, 0.f, 0.f, 1.f);
			vtx2d.UV = Vector2(1.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
			vtx2d.Color = Vector4(0.f, 0.f, 1.f, 1.f);
			vtx2d.UV = Vector2(0.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			// Crate Mesh
			std::shared_ptr<Mesh> RectMesh = std::make_shared<Mesh>();
			RectMesh->SetEngineDefaultRes(true);
			ResMgr::GetInst()->Add(strKey::Default::mesh::RectMesh, RectMesh);
			RectMesh->CreateVertexBuffer(VecVtx2D.data(), sizeof(vtx2d), VecVtx2D.size());

			std::vector<UINT> indices = { 0u , 1u, 2u, 0u, 2u, 3u, 0u };
			RectMesh->CreateIndexBuffer(indices.data(), (UINT)indices.size());

			VecVtx2D.clear();

#pragma endregion
#pragma region DEBUG RECTMESH

			vtx2d.Pos = Vector4(-0.5f, 0.5f, -0.00001f, 1.0f);
			vtx2d.Color = Vector4(0.f, 1.f, 0.f, 1.f);
			vtx2d.UV = Vector2(0.f, 0.f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = Vector4(0.5f, 0.5f, -0.00001f, 1.0f);
			vtx2d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			vtx2d.UV = Vector2(1.0f, 0.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = Vector4(0.5f, -0.5f, -0.00001f, 1.0f);
			vtx2d.Color = Vector4(1.f, 0.f, 0.f, 1.f);
			vtx2d.UV = Vector2(1.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.Pos = Vector4(-0.5f, -0.5f, -0.00001f, 1.0f);
			vtx2d.Color = Vector4(0.f, 0.f, 1.f, 1.f);
			vtx2d.UV = Vector2(0.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};
			
			// Create Mesh
			std::shared_ptr<Mesh> debugmesh = std::make_shared<Mesh>();
			ResMgr::GetInst()->Add(strKey::Default::mesh::DebugRectMesh, debugmesh);
			debugmesh->CreateVertexBuffer(VecVtx2D.data(), sizeof(Vertex2D), VecVtx2D.size());
			debugmesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
		}
#pragma endregion
#pragma region CIRCLE MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			std::vector<UINT>     VecIdx;

			Vertex2D center{};
			center.Pos = Vector4(0.0f, 0.0f, 0.f, 1.0f);
			center.Color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
			center.UV = Vector2(0.5f, 0.5f);
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
			VecIdx.push_back((UINT)VecVtx2D.size() - 1u);

			// Crate Mesh
			std::shared_ptr<Mesh> cirlceMesh = std::make_shared<Mesh>();
			ResMgr::GetInst()->Add(strKey::Default::mesh::CircleMesh, cirlceMesh);
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
			vtx3d.Pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			vtx3d.UV = Vector2(0.f, 0.f);
			vtx3d.Normal = Vector3(0.f, 1.f, 0.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			vtx3d.UV = Vector2(1.f, 0.f);
			vtx3d.Normal = Vector3(0.f, 1.f, 0.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			vtx3d.UV = Vector2(0.f, 1.f);
			vtx3d.Normal = Vector3(0.f, 1.f, 0.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			vtx3d.UV = Vector2(1.f, 1.f);
			vtx3d.Normal = Vector3(0.f, 1.f, 0.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 0.f, 0.f, 1.f);
			vtx3d.UV = Vector2(0.f, 0.f);
			vtx3d.Normal = Vector3(0.f, -1.f, 0.f);
			vtx3d.Tangent = Vector3(-1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 0.f, 0.f, 1.f);
			vtx3d.UV = Vector2(1.f, 0.f);
			vtx3d.Normal = Vector3(0.f, -1.f, 0.f);
			vtx3d.Tangent = Vector3(-1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 0.f, 0.f, 1.f);
			vtx3d.UV = Vector2(0.f, 1.f);
			vtx3d.Normal = Vector3(0.f, -1.f, 0.f);
			vtx3d.Tangent = Vector3(-1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 0.f, 0.f, 1.f);
			vtx3d.UV = Vector2(1.f, 1.f);
			vtx3d.Normal = Vector3(0.f, -1.f, 0.f);
			vtx3d.Tangent = Vector3(-1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(0.f, 1.f, 0.f, 1.f);
			vtx3d.UV = Vector2(0.f, 0.f);
			vtx3d.Normal = Vector3(-1.f, 0.f, 0.f);
			vtx3d.Tangent = Vector3(0.0f, 1.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(0.f, 1.f, 0.f, 1.f);
			vtx3d.UV = Vector2(1.f, 0.f);
			vtx3d.Normal = Vector3(-1.f, 0.f, 0.f);
			vtx3d.Tangent = Vector3(0.0f, 1.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(0.f, 1.f, 0.f, 1.f);
			vtx3d.UV = Vector2(0.f, 1.f);
			vtx3d.Normal = Vector3(-1.f, 0.f, 0.f);
			vtx3d.Tangent = Vector3(0.0f, 1.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(0.f, 1.f, 0.f, 1.f);
			vtx3d.UV = Vector2(1.f, 1.f);
			vtx3d.Normal = Vector3(-1.f, 0.f, 0.f);
			vtx3d.Tangent = Vector3(0.0f, 1.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(0.f, 0.f, 1.f, 1.f);
			vtx3d.UV = Vector2(0.f, 0.f);
			vtx3d.Normal = Vector3(1.f, 0.f, 0.f);
			vtx3d.Tangent = Vector3(0.0f, -1.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(0.f, 0.f, 1.f, 1.f);
			vtx3d.UV = Vector2(1.f, 0.f);
			vtx3d.Normal = Vector3(1.f, 0.f, 0.f);
			vtx3d.Tangent = Vector3(0.0f, -1.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(0.f, 0.f, 1.f, 1.f);
			vtx3d.UV = Vector2(0.f, 1.f);
			vtx3d.Normal = Vector3(1.f, 0.f, 0.f);
			vtx3d.Tangent = Vector3(0.0f, -1.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(0.f, 0.f, 1.f, 1.f);
			vtx3d.UV = Vector2(1.f, 1.f);
			vtx3d.Normal = Vector3(1.f, 0.f, 0.f);
			vtx3d.Tangent = Vector3(0.0f, -1.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 1.f, 0.f, 1.f);
			vtx3d.UV = Vector2(0.f, 0.f);
			vtx3d.Normal = Vector3(0.f, 0.f, 1.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 1.f, 0.f, 1.f);
			vtx3d.UV = Vector2(1.f, 0.f);
			vtx3d.Normal = Vector3(0.f, 0.f, 1.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 1.f, 0.f, 1.f);
			vtx3d.UV = Vector2(0.f, 1.f);
			vtx3d.Normal = Vector3(0.f, 0.f, 1.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 1.f, 0.f, 1.f);
			vtx3d.UV = Vector2(1.f, 1.f);
			vtx3d.Normal = Vector3(0.f, 0.f, 1.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);;
			vtx3d.Color = Vector4(1.f, 0.f, 1.f, 1.f);
			vtx3d.UV = Vector2(0.f, 0.f);
			vtx3d.Normal = Vector3(0.f, 0.f, -1.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 0.f, 1.f, 1.f);
			vtx3d.UV = Vector2(1.f, 0.f);
			vtx3d.Normal = Vector3(0.f, 0.f, -1.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 0.f, 1.f, 1.f);
			vtx3d.UV = Vector2(0.f, 1.f);
			vtx3d.Normal = Vector3(0.f, 0.f, -1.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.Pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.Color = Vector4(1.f, 0.f, 1.f, 1.f);
			vtx3d.UV = Vector2(1.f, 1.f);
			vtx3d.Normal = Vector3(0.f, 0.f, -1.f);
			vtx3d.Tangent = Vector3(1.0f, 0.0f, 0.0f);
			vtx3d.BiNormal = Vector3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			std::vector<UINT> indices;
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
			ResMgr::GetInst()->Add(strKey::Default::mesh::CubeMesh, cubMesh);
			cubMesh->Create<Vertex3D>(VecVtx3D, indices);
		}

#pragma endregion
#pragma region Sphere Mesh
		{
			Vertex3D vtx3d;
			std::vector<Vertex3D> VecVtx3D(2000);

			float fRadius = 0.5f;

			// Top
			vtx3d.Pos = Vector4(0.0f, fRadius, 0.0f, 1.0f);
			vtx3d.UV = Vector2(0.5f, 0.f);
			vtx3d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			vtx3d.Normal = Vector3(0.0f, fRadius, 0.0f);
			vtx3d.Normal.Normalize();
			vtx3d.Tangent = Vector3(1.f, 0.f, 0.f);
			vtx3d.BiNormal = Vector3(0.f, 0.f, 1.f);
			VecVtx3D.push_back(vtx3d);

			// Body
			UINT iStackCount = 40; // 가로 분할 개수
			UINT iSliceCount = 40; // 세로 분할 개수

			float fStackAngle = XM_PI / iStackCount;
			float fSliceAngle = XM_2PI / iSliceCount;

			float fUVXStep = 1.f / (float)iSliceCount;
			float fUVYStep = 1.f / (float)iStackCount;

			for (UINT i = 1; i < iStackCount; ++i)
			{
				float phi = i * fStackAngle;

				for (UINT j = 0; j <= iSliceCount; ++j)
				{
					float theta = j * fSliceAngle;

					vtx3d.Pos = Vector4(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
						, fRadius * cosf(i * fStackAngle)
						, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle), 1.0f);
					vtx3d.UV = Vector2(fUVXStep * j, fUVYStep * i);
					vtx3d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
					vtx3d.Normal = Vector3(vtx3d.Pos);
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
			vtx3d.Pos = Vector4(0.f, -fRadius, 0.f, 1.0f);
			vtx3d.UV = Vector2(0.5f, 1.f);
			vtx3d.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			vtx3d.Normal = Vector3(vtx3d.Pos.x, vtx3d.Pos.y, vtx3d.Pos.z);
			vtx3d.Normal.Normalize();

			vtx3d.Tangent = Vector3(1.f, 0.f, 0.f);
			vtx3d.BiNormal = Vector3(0.f, 0.f, -1.f);
			VecVtx3D.push_back(vtx3d);

			// 인덱스
			// 북극점
			std::vector<UINT> indices(2000);
			for (UINT i = 0; i < iSliceCount; ++i)
			{
				indices.push_back(0);
				indices.push_back(i + 2);
				indices.push_back(i + 1);
			}

			// 몸통
			for (UINT i = 0; i < iStackCount - 2; ++i)
			{
				for (UINT j = 0; j < iSliceCount; ++j)
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
			UINT iBottomIdx = (UINT)VecVtx3D.size() - 1;
			for (UINT i = 0; i < iSliceCount; ++i)
			{
				indices.push_back(iBottomIdx);
				indices.push_back(iBottomIdx - (i + 2));
				indices.push_back(iBottomIdx - (i + 1));
			}

			std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>();
			ResMgr::GetInst()->Add(strKey::Default::mesh::SphereMesh, sphereMesh);

			sphereMesh->Create<Vertex3D>(VecVtx3D, indices);
		}


#pragma endregion
	}

	void RenderMgr::LoadShader()
	{
#pragma region DEFAULT TRIANGLE SHADER
		{
			std::shared_ptr<GraphicsShader> TriangleShader = std::make_shared<GraphicsShader>();
			TriangleShader->CreateByHeader(eGSStage::VS, VS_Triangle, sizeof(VS_Triangle));
			TriangleShader->CreateByHeader(eGSStage::PS, PS_Triangle, sizeof(PS_Triangle));
			//shader->Create(eShaderStage::VS, "TriangleVS.hlsl", "main");
			//shader->Create(eShaderStage::PS, "TrianglePS.hlsl", "main");

			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::RectShader, TriangleShader);
		}
#pragma endregion
#pragma region SPRITE SHADER
		{
			std::shared_ptr<GraphicsShader> spriteShader = std::make_shared<GraphicsShader>();
			spriteShader->CreateByHeader(eGSStage::VS, VS_Sprite, sizeof(VS_Sprite));
			spriteShader->CreateByHeader(eGSStage::PS, PS_Sprite, sizeof(PS_Sprite));
			//spriteShader->Create(eShaderStage::VS, "SpriteVS.hlsl", "main");
			//spriteShader->Create(eShaderStage::PS, "SpritePS.hlsl", "main");
			spriteShader->SetRSState(eRSType::SolidNone);
			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::SpriteShader, spriteShader);
		}

#pragma endregion
#pragma region UI SHADER
		{
			std::shared_ptr<GraphicsShader> uiShader = std::make_shared<GraphicsShader>();
			uiShader->CreateByHeader(eGSStage::VS, VS_UserInterface, sizeof(VS_UserInterface));
			uiShader->CreateByHeader(eGSStage::PS, PS_UserInterface, sizeof(PS_UserInterface));
			//uiShader->Create(eShaderStage::VS, "UserInterfaceVS.hlsl", "main");
			//uiShader->Create(eShaderStage::PS, "UserInterfacePS.hlsl", "main");

			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::UIShader, uiShader);
		}

#pragma endregion
#pragma region GRID SHADER
		{
			std::shared_ptr<GraphicsShader> gridShader = std::make_shared<GraphicsShader>();
			gridShader->CreateByHeader(eGSStage::VS, VS_Grid, sizeof(VS_Grid));
			gridShader->CreateByHeader(eGSStage::PS, PS_Grid, sizeof(PS_Grid));

			//gridShader->Create(eShaderStage::VS, "GridVS.hlsl", "main");
			//gridShader->Create(eShaderStage::PS, "GridPS.hlsl", "main");
			gridShader->SetRSState(eRSType::SolidNone);
			gridShader->SetDSState(eDSType::NoWrite);
			gridShader->SetBSState(eBSType::AlphaBlend);

			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::GridShader, gridShader);
		}

#pragma endregion
#pragma region DEBUG SHADER
		{
			std::shared_ptr<GraphicsShader> debugShader = std::make_shared<GraphicsShader>();
			debugShader->CreateByHeader(eGSStage::VS, VS_Debug, sizeof(VS_Debug));
			debugShader->CreateByHeader(eGSStage::PS, PS_Debug, sizeof(PS_Debug));

			//debugShader->Create(eShaderStage::VS, "DebugVS.hlsl", "main");
			//debugShader->Create(eShaderStage::PS, "DebugPS.hlsl", "main");
			debugShader->SetRSState(eRSType::SolidNone);
			debugShader->SetDSState(eDSType::NoWrite);
			debugShader->SetBSState(eBSType::AlphaBlend);
			debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::DebugShader, debugShader);
		}

#pragma endregion
#pragma region PAINT SHADER
		{
			std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
			paintShader->CreateByHeader(CS_Paint, sizeof(CS_Paint));
			//paintShader->Create("PaintCS.hlsl", "main");
			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::PaintShader, paintShader);
		}

#pragma endregion
#pragma region PARTICLE SHADER
		{
			std::shared_ptr<GraphicsShader> particleShader = std::make_shared<GraphicsShader>();

			particleShader->CreateByHeader(eGSStage::VS, VS_Particle, sizeof(VS_Particle));
			particleShader->CreateByHeader(eGSStage::GS, GS_Particle, sizeof(GS_Particle));
			particleShader->CreateByHeader(eGSStage::PS, PS_Particle, sizeof(PS_Particle));
			//particleShader->Create(eShaderStage::VS, "ParticleVS.hlsl", "main");
			//particleShader->Create(eShaderStage::GS, "ParticleGS.hlsl", "main");
			//particleShader->Create(eShaderStage::PS, "ParticlePS.hlsl", "main");
			particleShader->SetRSState(eRSType::SolidNone);
			particleShader->SetDSState(eDSType::NoWrite);
			particleShader->SetBSState(eBSType::AlphaBlend);
			particleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::ParticleShader, particleShader);

			std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
			ResMgr::GetInst()->Add(strKey::Default::shader::compute::ParticleCS, particleCS);
			particleCS->CreateByHeader(CS_Particle, sizeof(CS_Particle));
		}
#pragma endregion
#pragma region POST PROCESS SHADER
		{
			std::shared_ptr<GraphicsShader> postProcessShader = std::make_shared<GraphicsShader>();
			postProcessShader->CreateByHeader(eGSStage::VS, VS_PostProcess, sizeof(VS_PostProcess));
			postProcessShader->CreateByHeader(eGSStage::PS, PS_PostProcess, sizeof(PS_PostProcess));
			//postProcessShader->Create(eShaderStage::VS, "PostProcessVS.hlsl", "main");
			//postProcessShader->Create(eShaderStage::PS, "PostProcessPS.hlsl", "main");
			postProcessShader->SetDSState(eDSType::NoWrite);
			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::PostProcessShader, postProcessShader);
		}

#pragma endregion
#pragma region BASIC 3D
		{
			std::shared_ptr<GraphicsShader> basic3DShader = std::make_shared<GraphicsShader>();
			basic3DShader->CreateByHeader(eGSStage::VS, VS_Basic, sizeof(VS_Basic));
			basic3DShader->CreateByHeader(eGSStage::PS, PS_Basic, sizeof(PS_Basic));
			ResMgr::GetInst()->Add(strKey::Default::shader::graphics::Basic3DShader, basic3DShader);
		}
#pragma endregion
	}

	void RenderMgr::SetupState()
	{
#pragma region Input layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc;
		D3D11_INPUT_ELEMENT_DESC LayoutDesc{};

		LayoutDesc.AlignedByteOffset = 0;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "POSITION";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		LayoutDesc.AlignedByteOffset = 16;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "COLOR";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		LayoutDesc.AlignedByteOffset = 32;
		LayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TEXCOORD";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		//Vector3 Tangent;
		//Vector3 BiNormal;
		//Vector3 Normal;
		using namespace strKey::Default::shader;
		std::shared_ptr<GraphicsShader> RectShader = ResMgr::GetInst()->Find<GraphicsShader>(graphics::RectShader);
		RectShader->CreateInputLayout(vecLayoutDesc);

		std::shared_ptr<GraphicsShader> spriteShader = ResMgr::GetInst()->Find<GraphicsShader>(graphics::SpriteShader);
		spriteShader->CreateInputLayout(vecLayoutDesc);

		std::shared_ptr<GraphicsShader> uiShader = ResMgr::GetInst()->Find<GraphicsShader>(graphics::UIShader);
		uiShader->CreateInputLayout(vecLayoutDesc);

		std::shared_ptr<GraphicsShader> gridShader = ResMgr::GetInst()->Find<GraphicsShader>(graphics::GridShader);
		uiShader->CreateInputLayout(vecLayoutDesc);

		std::shared_ptr<GraphicsShader> debugShader = ResMgr::GetInst()->Find<GraphicsShader>(graphics::DebugShader);
		debugShader->CreateInputLayout(vecLayoutDesc);

		std::shared_ptr<GraphicsShader> particleShader = ResMgr::GetInst()->Find<GraphicsShader>(graphics::ParticleShader);
		particleShader->CreateInputLayout(vecLayoutDesc);

		std::shared_ptr<GraphicsShader> postProcessShader = ResMgr::GetInst()->Find<GraphicsShader>(graphics::ParticleShader);
		postProcessShader->CreateInputLayout(vecLayoutDesc);


		LayoutDesc.AlignedByteOffset = 40;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TANGENT";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		LayoutDesc.AlignedByteOffset = 52;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BINORMAL";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		LayoutDesc.AlignedByteOffset = 64;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "NORMAL";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};


		std::shared_ptr<GraphicsShader> basicShader = ResMgr::GetInst()->Find<GraphicsShader>(graphics::Basic3DShader);
		basicShader->CreateInputLayout(vecLayoutDesc);

#pragma endregion


#pragma region sampler state
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

		GPUMgr::GetInst()->CreateSamplerState
		(
			&samplerDesc
			, mSamplerStates[(UINT)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		GPUMgr::GetInst()->CreateSamplerState
		(
			&samplerDesc
			, mSamplerStates[(UINT)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		GPUMgr::GetInst()->CreateSamplerState
		(
			&samplerDesc
			, mSamplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf()
		);

		GPUMgr::GetInst()->BindsSamplers((UINT)eSamplerType::Point
			, 1, mSamplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		GPUMgr::GetInst()->BindsSamplers((UINT)eSamplerType::Linear
			, 1, mSamplerStates[(UINT)eSamplerType::Linear].GetAddressOf());

		GPUMgr::GetInst()->BindsSamplers((UINT)eSamplerType::Anisotropic
			, 1, mSamplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
#pragma region Rasterizer state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

		GPUMgr::GetInst()->CreateRasterizerState(&rsDesc
			, mRasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		GPUMgr::GetInst()->CreateRasterizerState(&rsDesc
			, mRasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GPUMgr::GetInst()->CreateRasterizerState(&rsDesc
			, mRasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GPUMgr::GetInst()->CreateRasterizerState(&rsDesc
			, mRasterizerStates[(UINT)eRSType::WireframeNone].GetAddressOf());
#pragma endregion
#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GPUMgr::GetInst()->CreateDepthStencilState(&dsDesc
			, mDepthStencilStates[(UINT)eDSType::Less].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GPUMgr::GetInst()->CreateDepthStencilState(&dsDesc
			, mDepthStencilStates[(UINT)eDSType::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GPUMgr::GetInst()->CreateDepthStencilState(&dsDesc
			, mDepthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GPUMgr::GetInst()->CreateDepthStencilState(&dsDesc
			, mDepthStencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion
#pragma region Blend State
		//None
		mBlendStates[(UINT)eBSType::Default] = nullptr;

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

		GPUMgr::GetInst()->CreateBlendState(&bsDesc, mBlendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GPUMgr::GetInst()->CreateBlendState(&bsDesc, mBlendStates[(UINT)eBSType::OneOne].GetAddressOf());

#pragma endregion
	}

	void RenderMgr::LoadBuffer()
	{
#pragma region CONSTANT BUFFER
		mConstBuffers[(UINT)eCBType::Transform] = new ConstBuffer(eCBType::Transform);
		mConstBuffers[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		mConstBuffers[(UINT)eCBType::Material] = new ConstBuffer(eCBType::Material);
		mConstBuffers[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));

		mConstBuffers[(UINT)eCBType::Grid] = new ConstBuffer(eCBType::Grid);
		mConstBuffers[(UINT)eCBType::Grid]->Create(sizeof(GridCB));

		mConstBuffers[(UINT)eCBType::Animation] = new ConstBuffer(eCBType::Animation);
		mConstBuffers[(UINT)eCBType::Animation]->Create(sizeof(AnimationCB));

		mConstBuffers[(UINT)eCBType::Light] = new ConstBuffer(eCBType::Light);
		mConstBuffers[(UINT)eCBType::Light]->Create(sizeof(LightCB));

		mConstBuffers[(UINT)eCBType::ParticleSystem] = new ConstBuffer(eCBType::ParticleSystem);
		mConstBuffers[(UINT)eCBType::ParticleSystem]->Create(sizeof(ParticleSystemCB));

		mConstBuffers[(UINT)eCBType::Noise] = new ConstBuffer(eCBType::Noise);
		mConstBuffers[(UINT)eCBType::Noise]->Create(sizeof(NoiseCB));

#pragma endregion
#pragma region STRUCTED BUFFER
		mLightsBuffer = std::make_unique<StructBuffer>();
		mLightsBuffer->Create(sizeof(tLightAttribute), 128, eSRVType::SRV, nullptr, true);
#pragma endregion
	}

	void RenderMgr::LoadTexture()
	{
#pragma region STATIC TEXTURE
		using namespace strKey::Default;

		std::shared_ptr<Texture> pTex = nullptr;
		pTex = ResMgr::GetInst()->Load<Texture>(texture::DefaultSprite);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::HPBarTexture);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::CartoonSmoke);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::noise_01);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::noise_02);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::noise_03);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::BasicCube);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::BasicCubeNormal);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::Brick);
		assert(nullptr != pTex);

		pTex = ResMgr::GetInst()->Load<Texture>(texture::Brick_N);
		assert(nullptr != pTex);

#pragma endregion
#pragma region DYNAMIC TEXTURE
		std::shared_ptr<Texture> uavTexture = std::make_shared<Texture>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
			| D3D11_BIND_UNORDERED_ACCESS);
		ResMgr::GetInst()->Add(texture::PaintTexture, uavTexture);
#pragma endregion

		//noise
		std::shared_ptr<Texture> NoiseTex = std::make_shared<Texture>();
		NoiseTex->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		NoiseTex->BindShaderResource(eShaderStage::PS, 60);
	}

	void RenderMgr::LoadMaterial()
	{
		using namespace strKey::Default;
#pragma region DEFAULT
		std::shared_ptr <Texture> texture = ResMgr::GetInst()->Find<Texture>(texture::PaintTexture);
		texture->SetEngineDefaultRes(true);
		std::shared_ptr<GraphicsShader> shader = ResMgr::GetInst()->Find<GraphicsShader>(shader::graphics::RectShader);
		std::shared_ptr<Material> RectMaterial = std::make_shared<Material>();
		RectMaterial->SetShader(shader);
		RectMaterial->SetTexture(eTextureSlot::Albedo, texture);
		ResMgr::GetInst()->Add(material::RectMaterial, RectMaterial);
#pragma endregion
#pragma region SPRITE
		std::shared_ptr <Texture> spriteTexture = ResMgr::GetInst()->Find<Texture>(texture::DefaultSprite);
		std::shared_ptr<GraphicsShader> spriteShader = ResMgr::GetInst()->Find<GraphicsShader>(shader::graphics::SpriteShader);
		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
		spriteMaterial->SetShader(spriteShader);
		spriteMaterial->SetTexture(eTextureSlot::Albedo, spriteTexture);
		ResMgr::GetInst()->Add(material::SpriteMaterial, spriteMaterial);
#pragma endregion
#pragma region UI
		std::shared_ptr <Texture> uiTexture = ResMgr::GetInst()->Find<Texture>(texture::HPBarTexture);
		std::shared_ptr<GraphicsShader> uiShader = ResMgr::GetInst()->Find<GraphicsShader>(shader::graphics::UIShader);
		std::shared_ptr<Material> uiMaterial = std::make_shared<Material>();
		uiMaterial->SetRenderingMode(eRenderingMode::Transparent);

		uiMaterial->SetShader(uiShader);
		uiMaterial->SetTexture(eTextureSlot::Albedo, uiTexture);
		ResMgr::GetInst()->Add(material::UIMaterial, uiMaterial);
#pragma endregion
#pragma region GRID
		std::shared_ptr<GraphicsShader> gridShader = ResMgr::GetInst()->Find<GraphicsShader>(shader::graphics::GridShader);
		std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
		gridMaterial->SetShader(gridShader);
		ResMgr::GetInst()->Add(material::GridMaterial, gridMaterial);
#pragma endregion
#pragma region DEBUG
		std::shared_ptr<GraphicsShader> debugShader = ResMgr::GetInst()->Find<GraphicsShader>(shader::graphics::DebugShader);
		std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
		debugMaterial->SetRenderingMode(eRenderingMode::Transparent);
		debugMaterial->SetShader(debugShader);
		ResMgr::GetInst()->Add(material::DebugMaterial, debugMaterial);
#pragma endregion
#pragma region PARTICLE
		std::shared_ptr<GraphicsShader> particleShader = ResMgr::GetInst()->Find<GraphicsShader>(shader::graphics::ParticleShader);
		std::shared_ptr<Material> particleMaterial = std::make_shared<Material>();
		particleMaterial->SetRenderingMode(eRenderingMode::Transparent);
		particleMaterial->SetShader(particleShader);
		ResMgr::GetInst()->Add(material::ParticleMaterial, particleMaterial);
#pragma endregion
#pragma region POSTPROCESS
		std::shared_ptr<GraphicsShader> postProcessShader = ResMgr::GetInst()->Find<GraphicsShader>(shader::graphics::PostProcessShader);
		std::shared_ptr<Material> postProcessMaterial = std::make_shared<Material>();
		postProcessMaterial->SetRenderingMode(eRenderingMode::PostProcess);
		postProcessMaterial->SetShader(postProcessShader);
		ResMgr::GetInst()->Add(material::PostProcessMaterial, postProcessMaterial);
#pragma endregion

#pragma region BASIC
		std::shared_ptr<GraphicsShader> basic3DShader = ResMgr::GetInst()->Find<GraphicsShader>(shader::graphics::Basic3DShader);
		std::shared_ptr<Material> basic3DMaterial = std::make_shared<Material>();
		basic3DMaterial->SetRenderingMode(eRenderingMode::Transparent);
		basic3DMaterial->SetShader(basic3DShader);

		std::shared_ptr <Texture> albedo = ResMgr::GetInst()->Find<Texture>(texture::Brick);
		basic3DMaterial->SetTexture(eTextureSlot::Albedo, albedo);
		albedo = ResMgr::GetInst()->Find<Texture>(texture::Brick_N);
		basic3DMaterial->SetTexture(eTextureSlot::Normal, albedo);
		ResMgr::GetInst()->Add(material::Basic3DMaterial, basic3DMaterial);


#pragma endregion

	}
}

