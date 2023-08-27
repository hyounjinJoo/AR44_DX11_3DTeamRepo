#pragma once
#include "Entity.h"
#include "define_Struct.h"
#include "Animation3D.h"

//MeshData에 종속된 클래스
namespace mh
{
	class FBXLoader;
	class StructBuffer;
	class Skeleton :
		public Entity
	{
	public:
		Skeleton();
		virtual ~Skeleton();

		eResult Save(const std::filesystem::path& _fileName);
		eResult Load(const std::filesystem::path& _fileName);

		eResult CreateFromFBX(FBXLoader* _fbxLoader);

	public:
		//Animation 3D
		const std::vector<define::tMTBone>& GetBones() const { return m_vecBones; }
		UINT GetBoneCount() const { return (UINT)m_vecBones.size(); }
		StructBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset.get(); }	   // 각 뼈의 offset 행렬

		inline const std::unordered_map<std::string, Animation3D>&
			GetMapAnimations() const { return mMapAnimations; }

		const Animation3D* GetAnimation(const std::string& _strAnimName);

	private:
		void CreateBoneOffsetSBuffer();

	private:
		std::vector<define::tMTBone>			m_vecBones;
		std::unique_ptr<StructBuffer>			m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)

		std::unordered_map<std::string, Animation3D> mMapAnimations;
	};
}


