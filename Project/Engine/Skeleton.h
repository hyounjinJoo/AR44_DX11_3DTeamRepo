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

		const std::unordered_map<std::string, Animation3D*>& GetAnimations() const { return mMapAnimations; }
		bool IsAnimMesh() const { return (false == mMapAnimations.empty()); }

		inline const std::unordered_map<std::string, Animation3D*>&
			GetMapAnimations() const { return mMapAnimations; }

		const Animation3D* FindAnimation(const std::string& _strAnimName);


	private:
		void CreateBoneOffsetSBuffer();

	private:
		std::vector<define::tMTBone>					m_vecBones;
		std::unique_ptr<StructBuffer>					m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)


		std::unordered_map<std::string, Animation3D*>	mMapAnimations;
		//스켈레톤이 가지고 있는 애니메이션중 가장 많은 프레임수를 저장
		//Animator에서 구조화버퍼를 만들 떄 사용
		size_t											mAnimationMaxFrameSize;
	};
}


