#pragma once
#include "IRes.h"
#include "define_Struct.h"
#include "define_GPU.h"

namespace mh
{
	struct tMeshContainer
	{
		std::shared_ptr<Mesh>					pMesh;
		std::vector<std::shared_ptr<Material>>	pMaterials;
	};

	class Mesh;
	class Material;
	class GameObject;
	class Com_Renderer_Mesh;
	class Skeleton;
    class MeshData :
        public IRes
    {
	public:
		MeshData();
		virtual ~MeshData();

		virtual define::eResult Save(const std::filesystem::path& _fileName) override;
		virtual define::eResult Load(const std::filesystem::path& _fileName) override;

		virtual define::eResult SaveJson(Json::Value* _pJson) override;
		virtual define::eResult LoadJson(const Json::Value* _pJson) override;
		std::unique_ptr<GameObject> Instantiate();

	private:
		define::eResult LoadFromFBX(const std::filesystem::path& _fileName);
		bool SetRenderer(Com_Renderer_Mesh* _renderer, UINT _idx);

	private:
		std::vector<tMeshContainer> mMeshContainers;
		std::unique_ptr<Skeleton> mSkeleton;
    };

}


