#pragma once
#include "IRes.h"
#include "define_Struct.h"
#include "define_GPU.h"

namespace gui
{
	class guiFBXConverter;
}

namespace mh
{
	class Mesh;
	class Material;
	class GameObject;
	class Com_Renderer_Mesh;
	class Skeleton;
	struct tFBXMaterial;

	struct tMeshContainer
	{
		std::shared_ptr<Mesh>					pMesh;
		std::vector<std::shared_ptr<Material>>	pMaterials;
	};

    class MeshData :
        public IRes
    {
		friend class gui::guiFBXConverter;
		
	public:
		MeshData();
		virtual ~MeshData();

		virtual eResult Save(const std::fs::path& _filePath, const std::fs::path& _basePath = "") override;
		virtual eResult Load(const std::fs::path& _filePath, const std::fs::path& _basePath = "") override;
		

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;
		GameObject* Instantiate();

	private:
		eResult ConvertFBX(
			const std::fs::path& _fbxPath, bool _bStatic,
			const std::fs::path& _dirAndFileName);

		eResult LoadFromFBX(
			const std::fs::path& _fbxPath, bool _bStatic,
			const std::fs::path& _dirAndFileName);

		//기존에 존재하는 변환된 데이터에 애니메이션만 추가
		static eResult AddAnimationFromFBX(const std::fs::path& _fbxPath, const std::fs::path& _meshDataName);

		//_defaultMtrlStrKey: FBX 변환된 재질이 이름이 없을수도 있음 -> 만약 없을 경우 사용할 기본 키 이름
		std::shared_ptr<Material> ConvertMaterial(const tFBXMaterial* _material, const std::fs::path& _texDestDir);

		//몬스터헌터 텍스처 일부를 FBX에서 읽어주지 못해서 직접 등록해야함.
		//특히 노말맵이 등록이 안돼있어서 수동으로 등록해줘야됨.
		//다행히도 이름이 통일되어 있어서 이를 활용하면 가능할 듯
		void CheckMHMaterial(std::shared_ptr<Material> _mtrl, const std::fs::path& _texDestDir);

		bool SetRenderer(Com_Renderer_Mesh* _renderer, UINT _idx);

		

	private:
		std::vector<tMeshContainer> mMeshContainers;
		std::shared_ptr<Skeleton> mSkeleton;
    };

}


