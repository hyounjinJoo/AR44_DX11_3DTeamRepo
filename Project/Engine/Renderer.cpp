
#include "EnginePCH.h"

#include "define_Res.h"

#include "DefaultShaders.h"

#include "ResMgr.h"
#include "Renderer.h"
#include "TimeManager.h"

#include "Material.h"
#include "SceneManager.h"
#include "PaintShader.h"
#include "ParticleShader.h"

namespace mh::renderer
{
	Vertex gVertices[4] = {};
	ConstantBuffer* constantBuffers[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> gSamplerStates[(UINT)eSamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> gRasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> gDepthStencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> gBlendStates[(UINT)eBSType::End] = {};
	
	Camera* gMainCamera = nullptr;
	std::vector<Camera*> gCameras[(UINT)eSceneType::End];
	std::vector<tDebugMesh> gDebugMeshes;
	std::vector<tLightAttribute> gLights;
	StructedBuffer* gLightsBuffer = nullptr;

	std::shared_ptr<Texture> postProcessTexture = nullptr;
	mh::GameObject* gInspectorGameObject = nullptr;


	void LoadMesh()
	{
		using namespace mh::define;
		Vertex v = {};

#pragma region POINT MESH
		{
			std::shared_ptr<Mesh> pointMesh = std::make_shared<Mesh>();
			pointMesh->SetEngineDefaultRes(true);
			ResMgr::GetInst()->Add(strKey::Default::mesh::PointMesh, pointMesh);

			pointMesh->CreateVertexBuffer(&v, 1);
			UINT pointIndex = 0;
			pointMesh->CreateIndexBuffer(&pointIndex, 1);
		}
		
#pragma endregion
#pragma region RECT MESH
		{
			//RECT
			gVertices[0].Pos = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
			gVertices[0].Color = Vector4(0.f, 1.f, 0.f, 1.f);
			gVertices[0].UV = Vector2(0.f, 0.f);

			gVertices[1].Pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
			gVertices[1].Color = Vector4(1.f, 1.f, 1.f, 1.f);
			gVertices[1].UV = Vector2(1.0f, 0.0f);

			gVertices[2].Pos = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
			gVertices[2].Color = Vector4(1.f, 0.f, 0.f, 1.f);
			gVertices[2].UV = Vector2(1.0f, 1.0f);

			gVertices[3].Pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
			gVertices[3].Color = Vector4(0.f, 0.f, 1.f, 1.f);
			gVertices[3].UV = Vector2(0.0f, 1.0f);

			// Crate Mesh
			std::shared_ptr<Mesh> RectMesh = std::make_shared<Mesh>();
			RectMesh->SetEngineDefaultRes(true);
			ResMgr::GetInst()->Add(strKey::Default::mesh::RectMesh, RectMesh);
			RectMesh->CreateVertexBuffer(gVertices, 4);

			std::vector<UINT> indices;
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(0);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(0);
			RectMesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));

#pragma endregion
#pragma region DEBUG RECTMESH

			gVertices[0].Pos = Vector4(-0.5f, 0.5f, -0.00001f, 1.0f);
			gVertices[0].Color = Vector4(0.f, 1.f, 0.f, 1.f);
			gVertices[0].UV = Vector2(0.f, 0.f);

			gVertices[1].Pos = Vector4(0.5f, 0.5f, -0.00001f, 1.0f);
			gVertices[1].Color = Vector4(1.f, 1.f, 1.f, 1.f);
			gVertices[1].UV = Vector2(1.0f, 0.0f);

			gVertices[2].Pos = Vector4(0.5f, -0.5f, -0.00001f, 1.0f);
			gVertices[2].Color = Vector4(1.f, 0.f, 0.f, 1.f);
			gVertices[2].UV = Vector2(1.0f, 1.0f);

			gVertices[3].Pos = Vector4(-0.5f, -0.5f, -0.00001f, 1.0f);
			gVertices[3].Color = Vector4(0.f, 0.f, 1.f, 1.f);
			gVertices[3].UV = Vector2(0.0f, 1.0f);

			// Create Mesh
			std::shared_ptr<Mesh> debugmesh = std::make_shared<Mesh>();
			ResMgr::GetInst()->Add(strKey::Default::mesh::DebugRectMesh, debugmesh);
			debugmesh->CreateVertexBuffer(gVertices, 4);
			debugmesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
		}
#pragma endregion
#pragma region CIRCLE MESH
		{
			std::vector<Vertex> circleVtxes;
			Vertex center = {};
			center.Pos = Vector4(0.0f, 0.0f, -0.00001f, 1.0f);
			center.Color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
			center.UV = Vector2::Zero;

			circleVtxes.push_back(center);

			int iSlice = 80;
			float fRadius = 0.5f;
			float fTheta = XM_2PI / (float)iSlice;


			for (size_t i = 0; i < iSlice; i++)
			{
				Vertex vtx = {};
				vtx.Pos = Vector4
				(
					fRadius * cosf(fTheta * (float)i)
					, fRadius * sinf(fTheta * (float)i)
					, -0.00001f, 1.0f
				);
				vtx.Color = center.Color;

				circleVtxes.push_back(vtx);
			}

			std::vector<UINT> indices;
			for (size_t i = 0; i < static_cast<size_t>(iSlice) - 2; i++)
			{
				indices.push_back(static_cast<int>(i) + 1);
			}
			indices.push_back(1);

			// Crate Mesh
			std::shared_ptr<Mesh> cirlceMesh = std::make_shared<Mesh>();
			ResMgr::GetInst()->Add(strKey::Default::mesh::CircleMesh, cirlceMesh);
			cirlceMesh->CreateVertexBuffer(circleVtxes.data(), static_cast<UINT>(circleVtxes.size()));
			cirlceMesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
		}
#pragma endregion
#pragma region Cube Mesh
		{
			Vertex arrCube[24] = {};

			// 윗면
			arrCube[0].Pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
			arrCube[0].Color = Vector4(1.f, 1.f, 1.f, 1.f);
			arrCube[0].UV = Vector2(0.f, 0.f);
			arrCube[0].Normal = Vector3(0.f, 1.f, 0.f);
			arrCube[0].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[0].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[1].Pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			arrCube[1].Color = Vector4(1.f, 1.f, 1.f, 1.f);
			arrCube[1].UV = Vector2(1.f, 0.f);
			arrCube[1].Normal = Vector3(0.f, 1.f, 0.f);
			arrCube[1].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[1].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[2].Pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
			arrCube[2].Color = Vector4(1.f, 1.f, 1.f, 1.f);
			arrCube[2].UV = Vector2(0.f, 1.f);
			arrCube[2].Normal = Vector3(0.f, 1.f, 0.f);
			arrCube[2].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[2].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[3].Pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
			arrCube[3].Color = Vector4(1.f, 1.f, 1.f, 1.f);
			arrCube[3].UV = Vector2(1.f, 1.f);
			arrCube[3].Normal = Vector3(0.f, 1.f, 0.f);
			arrCube[3].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[3].BiNormal = Vector3(0.0f, 0.0f, 1.0f);


			// 아랫 면	
			arrCube[4].Pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			arrCube[4].Color = Vector4(1.f, 0.f, 0.f, 1.f);
			arrCube[4].UV = Vector2(0.f, 0.f);
			arrCube[4].Normal = Vector3(0.f, -1.f, 0.f);
			arrCube[4].Tangent = Vector3(-1.0f, 0.0f, 0.0f);
			arrCube[4].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[5].Pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
			arrCube[5].Color = Vector4(1.f, 0.f, 0.f, 1.f);
			arrCube[5].UV = Vector2(1.f, 0.f);
			arrCube[5].Normal = Vector3(0.f, -1.f, 0.f);
			arrCube[5].Tangent = Vector3(-1.0f, 0.0f, 0.0f);
			arrCube[5].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[6].Pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
			arrCube[6].Color = Vector4(1.f, 0.f, 0.f, 1.f);
			arrCube[6].UV = Vector2(0.f, 1.f);
			arrCube[6].Normal = Vector3(0.f, -1.f, 0.f);
			arrCube[6].Tangent = Vector3(-1.0f, 0.0f, 0.0f);
			arrCube[6].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[7].Pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
			arrCube[7].Color = Vector4(1.f, 0.f, 0.f, 1.f);
			arrCube[7].UV = Vector2(1.f, 1.f);
			arrCube[7].Normal = Vector3(0.f, -1.f, 0.f);
			arrCube[7].Tangent = Vector3(-1.0f, 0.0f, 0.0f);
			arrCube[7].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			// 왼쪽 면
			arrCube[8].Pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
			arrCube[8].Color = Vector4(0.f, 1.f, 0.f, 1.f);
			arrCube[8].UV = Vector2(0.f, 0.f);
			arrCube[8].Normal = Vector3(-1.f, 0.f, 0.f);
			arrCube[8].Tangent = Vector3(0.0f, 1.0f, 0.0f);
			arrCube[8].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[9].Pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
			arrCube[9].Color = Vector4(0.f, 1.f, 0.f, 1.f);
			arrCube[9].UV = Vector2(1.f, 0.f);
			arrCube[9].Normal = Vector3(-1.f, 0.f, 0.f);
			arrCube[9].Tangent = Vector3(0.0f, 1.0f, 0.0f);
			arrCube[9].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[10].Pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			arrCube[10].Color = Vector4(0.f, 1.f, 0.f, 1.f);
			arrCube[10].UV = Vector2(0.f, 1.f);
			arrCube[10].Normal = Vector3(-1.f, 0.f, 0.f);
			arrCube[10].Tangent = Vector3(0.0f, 1.0f, 0.0f);
			arrCube[10].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[11].Pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
			arrCube[11].Color = Vector4(0.f, 1.f, 0.f, 1.f);
			arrCube[11].UV = Vector2(1.f, 1.f);
			arrCube[11].Normal = Vector3(-1.f, 0.f, 0.f);
			arrCube[11].Tangent = Vector3(0.0f, 1.0f, 0.0f);
			arrCube[11].BiNormal = Vector3(0.0f, 0.0f, 1.0f);


			// 오른쪽 면
			arrCube[12].Pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
			arrCube[12].Color = Vector4(0.f, 0.f, 1.f, 1.f);
			arrCube[12].UV = Vector2(0.f, 0.f);
			arrCube[12].Normal = Vector3(1.f, 0.f, 0.f);
			arrCube[12].Tangent = Vector3(0.0f, -1.0f, 0.0f);
			arrCube[12].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[13].Pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			arrCube[13].Color = Vector4(0.f, 0.f, 1.f, 1.f);
			arrCube[13].UV = Vector2(1.f, 0.f);
			arrCube[13].Normal = Vector3(1.f, 0.f, 0.f);
			arrCube[13].Tangent = Vector3(0.0f, -1.0f, 0.0f);
			arrCube[13].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[14].Pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
			arrCube[14].Color = Vector4(0.f, 0.f, 1.f, 1.f);
			arrCube[14].UV = Vector2(0.f, 1.f);
			arrCube[14].Normal = Vector3(1.f, 0.f, 0.f);
			arrCube[14].Tangent = Vector3(0.0f, -1.0f, 0.0f);
			arrCube[14].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			arrCube[15].Pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
			arrCube[15].Color = Vector4(0.f, 0.f, 1.f, 1.f);
			arrCube[15].UV = Vector2(1.f, 1.f);
			arrCube[15].Normal = Vector3(1.f, 0.f, 0.f);
			arrCube[15].Tangent = Vector3(0.0f, -1.0f, 0.0f);
			arrCube[15].BiNormal = Vector3(0.0f, 0.0f, 1.0f);

			// 뒷 면
			arrCube[16].Pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			arrCube[16].Color = Vector4(1.f, 1.f, 0.f, 1.f);
			arrCube[16].UV = Vector2(0.f, 0.f);
			arrCube[16].Normal = Vector3(0.f, 0.f, 1.f);
			arrCube[16].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[16].BiNormal = Vector3(0.0f, -1.0f, 1.0f);

			arrCube[17].Pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
			arrCube[17].Color = Vector4(1.f, 1.f, 0.f, 1.f);
			arrCube[17].UV = Vector2(1.f, 0.f);
			arrCube[17].Normal = Vector3(0.f, 0.f, 1.f);
			arrCube[17].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[17].BiNormal = Vector3(0.0f, -1.0f, 1.0f);

			arrCube[18].Pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
			arrCube[18].Color = Vector4(1.f, 1.f, 0.f, 1.f);
			arrCube[18].UV = Vector2(0.f, 1.f);
			arrCube[18].Normal = Vector3(0.f, 0.f, 1.f);
			arrCube[18].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[18].BiNormal = Vector3(0.0f, -1.0f, 1.0f);

			arrCube[19].Pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
			arrCube[19].Color = Vector4(1.f, 1.f, 0.f, 1.f);
			arrCube[19].UV = Vector2(1.f, 1.f);
			arrCube[19].Normal = Vector3(0.f, 0.f, 1.f);
			arrCube[19].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[19].BiNormal = Vector3(0.0f, -1.0f, 1.0f);

			// 앞 면
			arrCube[20].Pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);;
			arrCube[20].Color = Vector4(1.f, 0.f, 1.f, 1.f);
			arrCube[20].UV = Vector2(0.f, 0.f);
			arrCube[20].Normal = Vector3(0.f, 0.f, -1.f);
			arrCube[20].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[20].BiNormal = Vector3(0.0f, 1.0f, 1.0f);

			arrCube[21].Pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
			arrCube[21].Color = Vector4(1.f, 0.f, 1.f, 1.f);
			arrCube[21].UV = Vector2(1.f, 0.f);
			arrCube[21].Normal = Vector3(0.f, 0.f, -1.f);
			arrCube[21].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[21].BiNormal = Vector3(0.0f, 1.0f, 1.0f);

			arrCube[22].Pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
			arrCube[22].Color = Vector4(1.f, 0.f, 1.f, 1.f);
			arrCube[22].UV = Vector2(0.f, 1.f);
			arrCube[22].Normal = Vector3(0.f, 0.f, -1.f);
			arrCube[22].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[22].BiNormal = Vector3(0.0f, 1.0f, 1.0f);

			arrCube[23].Pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			arrCube[23].Color = Vector4(1.f, 0.f, 1.f, 1.f);
			arrCube[23].UV = Vector2(1.f, 1.f);
			arrCube[23].Normal = Vector3(0.f, 0.f, -1.f);
			arrCube[23].Tangent = Vector3(1.0f, 0.0f, 0.0f);
			arrCube[23].BiNormal = Vector3(0.0f, 1.0f, 1.0f);

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
			cubMesh->CreateVertexBuffer(arrCube, 24);
			cubMesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
		}

#pragma endregion
#pragma region Sphere Mesh
		{
			v = {};
			float fRadius = 0.5f;
			std::vector<Vertex> sphereVtx;

			// Top
			v.Pos = Vector4(0.0f, fRadius, 0.0f, 1.0f);
			v.UV = Vector2(0.5f, 0.f);
			v.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			v.Normal = Vector3(0.0f, fRadius, 0.0f);
			v.Normal.Normalize();
			v.Tangent = Vector3(1.f, 0.f, 0.f);
			v.BiNormal = Vector3(0.f, 0.f, 1.f);
			sphereVtx.push_back(v);

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

					v.Pos = Vector4(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
						, fRadius * cosf(i * fStackAngle)
						, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle), 1.0f);
					v.UV = Vector2(fUVXStep * j, fUVYStep * i);
					v.Color = Vector4(1.f, 1.f, 1.f, 1.f);
					v.Normal = Vector3(v.Pos.x, v.Pos.y, v.Pos.z);
					//v.Normal.Normalize();

					v.Tangent.x = -fRadius * sinf(phi) * sinf(theta);
					v.Tangent.y = 0.f;
					v.Tangent.z = fRadius * sinf(phi) * cosf(theta);
					v.Tangent.Normalize();

					v.Tangent.Cross(v.Normal, v.BiNormal);
					v.BiNormal.Normalize();

					sphereVtx.push_back(v);
				}
			}

			// Bottom
			v.Pos = Vector4(0.f, -fRadius, 0.f, 1.0f);
			v.UV = Vector2(0.5f, 1.f);
			v.Color = Vector4(1.f, 1.f, 1.f, 1.f);
			v.Normal = Vector3(v.Pos.x, v.Pos.y, v.Pos.z);
			v.Normal.Normalize();

			v.Tangent = Vector3(1.f, 0.f, 0.f);
			v.BiNormal = Vector3(0.f, 0.f, -1.f);
			sphereVtx.push_back(v);

			// 인덱스
			// 북극점
			std::vector<UINT> indices;
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
			UINT iBottomIdx = (UINT)sphereVtx.size() - 1;
			for (UINT i = 0; i < iSliceCount; ++i)
			{
				indices.push_back(iBottomIdx);
				indices.push_back(iBottomIdx - (i + 2));
				indices.push_back(iBottomIdx - (i + 1));
			}

			std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>();
			ResMgr::GetInst()->Add(strKey::Default::mesh::SphereMesh, sphereMesh);
			sphereMesh->CreateVertexBuffer(sphereVtx.data(), static_cast<UINT>(sphereVtx.size()));
			sphereMesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
		}


#pragma endregion
}

	void LoadShader()
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

	void SetUpState()
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

		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, gSamplerStates[(UINT)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, gSamplerStates[(UINT)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, gSamplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf()
		);

		GetDevice()->BindsSamplers((UINT)eSamplerType::Point
			, 1, gSamplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Linear
			, 1, gSamplerStates[(UINT)eSamplerType::Linear].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Anisotropic
			, 1, gSamplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
		#pragma region Rasterizer state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

		GetDevice()->CreateRasterizerState(&rsDesc
			, gRasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		GetDevice()->CreateRasterizerState(&rsDesc
			, gRasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc
			, gRasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc
			, gRasterizerStates[(UINT)eRSType::WireframeNone].GetAddressOf());
#pragma endregion
		#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		
		GetDevice()->CreateDepthStencilState(&dsDesc
			, gDepthStencilStates[(UINT)eDSType::Less].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, gDepthStencilStates[(UINT)eDSType::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, gDepthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, gDepthStencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion
		#pragma region Blend State
		//None
		gBlendStates[(UINT)eBSType::Default] = nullptr;

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

		GetDevice()->CreateBlendState(&bsDesc, gBlendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&bsDesc, gBlendStates[(UINT)eBSType::OneOne].GetAddressOf());

#pragma endregion
	}

	void LoadBuffer()
	{
		#pragma region CONSTANT BUFFER
		constantBuffers[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));

		constantBuffers[(UINT)eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[(UINT)eCBType::Grid]->Create(sizeof(GridCB));

		constantBuffers[(UINT)eCBType::Animation] = new ConstantBuffer(eCBType::Animation);
		constantBuffers[(UINT)eCBType::Animation]->Create(sizeof(AnimationCB));

		constantBuffers[(UINT)eCBType::Light] = new ConstantBuffer(eCBType::Light);
		constantBuffers[(UINT)eCBType::Light]->Create(sizeof(LightCB));

		constantBuffers[(UINT)eCBType::ParticleSystem] = new ConstantBuffer(eCBType::ParticleSystem);
		constantBuffers[(UINT)eCBType::ParticleSystem]->Create(sizeof(ParticleSystemCB));

		constantBuffers[(UINT)eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
		constantBuffers[(UINT)eCBType::Noise]->Create(sizeof(NoiseCB));

#pragma endregion
		#pragma region STRUCTED BUFFER
		gLightsBuffer = new StructedBuffer();
		gLightsBuffer->Create(sizeof(tLightAttribute), 128, eSRVType::SRV, nullptr, true);
#pragma endregion
	}

	void LoadTexture()
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
		postProcessTexture = std::make_shared<Texture>();
		postProcessTexture->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		postProcessTexture->BindShaderResource(eShaderStage::PS, 60);

	}

	void LoadMaterial()
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
		std::shared_ptr <Texture> spriteTexture= ResMgr::GetInst()->Find<Texture>(texture::DefaultSprite);
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

	void Initialize()
	{
		LoadMesh();
		LoadShader();
		SetUpState();
		LoadBuffer();
		LoadTexture();
		LoadMaterial();
	}

	void Release()
	{
		for (size_t i = 0; i < (UINT)eCBType::End; i++)
		{
			delete constantBuffers[i]; 
			constantBuffers[i] = nullptr;
		}

		delete gLightsBuffer;
		gLightsBuffer = nullptr;
	}

	void Render()
	{
		BindNoiseTexture();
		BindLights();
		
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		for (Camera* cam : gCameras[(UINT)type])
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		gCameras[(UINT)type].clear();
		renderer::gLights.clear();
	}

	void PushLightAttribute(const tLightAttribute& lightAttribute)
	{
		gLights.push_back(lightAttribute);
	}

	void BindLights()
	{
		gLightsBuffer->SetData(gLights.data(), static_cast<UINT>(gLights.size()));
		gLightsBuffer->BindSRV(eShaderStage::VS, 13);
		gLightsBuffer->BindSRV(eShaderStage::PS, 13);

		renderer::LightCB trCb = {};
		trCb.NumberOfLight = static_cast<UINT>(gLights.size());

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Light];
		cb->SetData(&trCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	float NoiseTime = 10.0f;
	void BindNoiseTexture()
	{
		std::shared_ptr<Texture> noise = ResMgr::GetInst()->Find<Texture>(strKey::Default::texture::noise_03);
		noise->BindShaderResource(eShaderStage::VS, 16);
		noise->BindShaderResource(eShaderStage::HS, 16);
		noise->BindShaderResource(eShaderStage::DS, 16);
		noise->BindShaderResource(eShaderStage::GS, 16);
		noise->BindShaderResource(eShaderStage::PS, 16);
		noise->BindShaderResource(eShaderStage::CS, 16);

		NoiseCB info = {};
		info.NoiseSize.x = static_cast<float>(noise->GetWidth());
		info.NoiseSize.y = static_cast<float>(noise->GetHeight());
		NoiseTime -= TimeManager::DeltaTime();
		info.NoiseTime = NoiseTime;

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Noise];
		cb->SetData(&info);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}

	void CopyRenderTarget()
	{
		std::shared_ptr<Texture> renderTarget = ResMgr::GetInst()->Find<Texture>(strKey::Default::texture::RenderTarget);

		ID3D11ShaderResourceView* srv = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::PS, 60, &srv);

		ID3D11Texture2D* dest = postProcessTexture->GetTexture().Get();
		ID3D11Texture2D* source = renderTarget->GetTexture().Get();

		GetDevice()->CopyResource(dest, source);

		postProcessTexture->BindShaderResource(eShaderStage::PS, 60);
	}
}
