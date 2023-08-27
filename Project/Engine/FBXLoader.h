#pragma once
#include "Entity.h"

#include <windef.h>
#include "SimpleMath.h"

#include <fbxsdk/fbxsdk.h>


namespace mh
{
//===============
// Struct of FBX 
//===============
// Material 계수

	struct tFBXMaterial
	{
		std::string		strMtrlName{};
		std::string		strDiffuseTex{};
		std::string		strNormalTex{};
		std::string		strSpecularTex{};
		std::string		strEmissiveTex{};

		float4 DiffuseColor{};
		float4 SpecularColor{};
		float4 AmbientColor{};
		float4 EmissiveColor{};

		float	SpecularPower{};
		float	TransparencyFactor{};
		float	Shininess{};
	};

	struct tFBXWeight
	{
		int		BoneIdx{};
		double	Weight{};
	};

	struct tFBXContainer
	{
		std::string								Name{};
		std::vector<float3>						vecPosition{};
		std::vector<float2>						vecUV{};

		std::vector<float3>						vecTangent{};
		std::vector<float3>						vecBinormal{};
		std::vector<float3>						vecNormal{};

		std::vector<float4>						vecBlendIndex{};
		std::vector<float4>						vecBlendWeight{};

		std::vector<std::vector<UINT>>			vecIndexBuffers{};
		std::vector<tFBXMaterial>				vecMtrl{};

		//이 정점이 특정 Bone Index로부터 얼마만큼의 추가 가중치를 받을것인지(배율)
		std::unordered_map<int, std::vector<tFBXWeight>>	mapWeights{};

		bool bBump{};
		bool bAnimation{};

		void ResizeVertices(int _iSize)
		{
			vecPosition.resize(_iSize);
			vecUV.resize(_iSize);
			vecTangent.resize(_iSize);
			vecBinormal.resize(_iSize);
			vecNormal.resize(_iSize);
		}
	};

	struct tFBXBone
	{
		std::string					strBoneName{};
		int							Depth{};			// 계층구조 깊이
		int							ParentIndx{};	// 부모 Bone 의 인덱스
		fbxsdk::FbxAMatrix			matOffset{};		// Offset 행렬( -> 뿌리 -> Local)
		fbxsdk::FbxAMatrix			matBone{};
	};

	struct tFBXKeyFrame
	{
		fbxsdk::FbxAMatrix  matTransform{};
		double				Time{};
	};

	struct tFBXKeyFramesPerBone
	{
		int							BoneIndex;
		std::vector<tFBXKeyFrame>	vecKeyFrame;
	};


	struct tFBXAnimClip
	{
		std::string		strName{};
		fbxsdk::FbxTime		StartTime{};
		fbxsdk::FbxTime		EndTime{};
		
		fbxsdk::FbxLongLong	TimeLength{};
		fbxsdk::FbxTime::EMode TimeMode{};

		//각 본 별 키프레임 데이터
		std::vector<tFBXKeyFramesPerBone> KeyFramesPerBone{};
	};

	class FBXLoader :
		public Entity
	{
	public:
		FBXLoader();
		virtual ~FBXLoader();

	public:
		void Reset();

		eResult LoadFbx(const std::filesystem::path& _strPath, bool _bStatic = true);

		// FbxMatrix -> Matrix(FBX(double) 포맷에서 프로젝트 포맷(float)으로 변환해서 반환해준다
		static MATRIX GetMatrixFromFbxMatrix(const fbxsdk::FbxAMatrix& _mat);

		const std::vector<tFBXContainer>& GetContainers() const { return mContainers; }
		const std::vector<tFBXBone>& GetBones() const { return mBones; }
		const std::vector<tFBXAnimClip>& GetAnimClip() const { return mAnimClips; }

	private:
		void LoadMeshContainer(fbxsdk::FbxNode* _pRoot, bool _bStatic);
		void LoadMesh(fbxsdk::FbxMesh* _pFBXMesh, bool _bStatic);
		void LoadMaterial(fbxsdk::FbxSurfaceMaterial* _pMtrlSur);

		void GetTangent(fbxsdk::FbxMesh* _pMesh, tFBXContainer& _pContainer, int _vertexIdx, int _iVtxOrder);
		void GetBinormal(fbxsdk::FbxMesh* _pMesh, tFBXContainer& _pContainer, int _vertexIdx, int _iVtxOrder);
		void GetNormal(fbxsdk::FbxMesh* _pMesh, tFBXContainer& _pContainer, int _vertexIdx, int _iVtxOrder);
		void GetUV(fbxsdk::FbxMesh* _pMesh, tFBXContainer& _pContainer, int _vertexIdx, int _iVtxOrder);

		float4 GetMtrlData(fbxsdk::FbxSurfaceMaterial* _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName);
		std::string GetMtrlTextureName(fbxsdk::FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty);

		void LoadTexture();

		//혹시라도 Material 이름이 안지어져있을 때 사용하기위해 Path를 받아옴(이름이 있을경우 사용 X)
		void CreateMaterial(const std::filesystem::path& _strPath);

		// Animation
		void LoadSkeleton(fbxsdk::FbxNode* _pNode);
		void LoadBoneRecursive(fbxsdk::FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
		void LoadAnimationClip();
		void Triangulate(fbxsdk::FbxNode* _pNode);

		void LoadAnimationData(fbxsdk::FbxMesh* _pMesh, tFBXContainer& _pContainer);
		void LoadWeightsAndIndices(fbxsdk::FbxCluster* _pCluster, int _bondIndex, tFBXContainer& _pContainer);
		void LoadOffsetMatrix(fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _matNodeTransform, int _bondIndex, tFBXContainer& _pContainer);
		void LoadKeyframeTransform(fbxsdk::FbxNode* _pNode, fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _matNodeTransform
			, int _bondIndex, tFBXContainer& _pContainer);

		int FindBoneIndex(const std::string& _strBoneName);
		fbxsdk::FbxAMatrix GetTransform(fbxsdk::FbxNode* _pNode);

		void CheckWeightAndIndices(fbxsdk::FbxMesh* _pMesh, tFBXContainer& _pContainer);

		int GetAllNodesCountRecursive(fbxsdk::FbxNode* _pNode);

	private:
		fbxsdk::FbxManager*		mManager;
		fbxsdk::FbxScene*			mScene;

		std::vector<tFBXContainer>				mContainers;

		// Animation
		std::vector<tFBXBone>					mBones;
		fbxsdk::FbxArray<fbxsdk::FbxString*>	mAnimNames;
		std::vector<tFBXAnimClip>				mAnimClips;
		bool									mbMixamo;
	};
}


