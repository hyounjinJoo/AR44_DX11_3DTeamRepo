#pragma once
#include "IRes.h"
#include "define_Struct.h"
#include "define_GPU.h"

namespace mh
{
	class Mesh;
	class Material;
	class GameObject;
	class Com_Renderer_Mesh;
	class Skeleton;

	struct tMeshContainer
	{
		std::shared_ptr<Mesh>					pMesh;
		std::vector<std::shared_ptr<Material>>	pMaterials;
	};

    class MeshData :
        public IRes
    {
	public:
		MeshData();
		virtual ~MeshData();

		virtual eResult Save(const std::filesystem::path& _fileName) override;
		virtual eResult Load(const std::filesystem::path& _fileName) override;

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;
		GameObject* Instantiate();

	private:
		eResult LoadFromFBX(const std::filesystem::path& _fileName);
		bool SetRenderer(Com_Renderer_Mesh* _renderer, UINT _idx);

	private:
		std::vector<tMeshContainer> mMeshContainers;
		std::unique_ptr<Skeleton> mSkeleton;
    };

}


