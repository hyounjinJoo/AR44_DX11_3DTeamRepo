#pragma once
#include "Entity.h"
#include "SimpleMath.h"

#include <fbxsdk/fbxsdk.h>


namespace mh
{
	//===============
// Struct of FBX 
//===============
// Material 계수
	struct tMtrlData
	{
		float4 vDiff;
		float4 vSpec;
		float4 vAmb;
		float4 vEmv;
	};

	struct tFbxMaterial
	{
		tMtrlData			tMtrl;
		std::string		strMtrlName;
		std::string		strDiff;
		std::string		strNormal;
		std::string		strSpec;
		std::string		strEmis;
	};

	struct tWeightsAndIndices
	{
		int		iBoneIdx;
		double	dWeight;
	};

	struct tContainer
	{
		std::string								strName;
		std::vector<float3>						vecPos;
		std::vector<float3>						vecTangent;
		std::vector<float3>						vecBinormal;
		std::vector<float3>						vecNormal;
		std::vector<float2>						vecUV;

		std::vector<float4>						vecIndices;
		std::vector<float4>						vecWeights;

		std::vector<std::vector<UINT>>				vecIdx;
		std::vector<tFbxMaterial>				vecMtrl;

		// Animation 관련 정보
		bool									bAnimation;
		std::vector<std::vector<tWeightsAndIndices>>	vecWI;

		void Resize(UINT _iSize)
		{
			vecPos.resize(_iSize);
			vecTangent.resize(_iSize);
			vecBinormal.resize(_iSize);
			vecNormal.resize(_iSize);
			vecUV.resize(_iSize);
			vecIndices.resize(_iSize);
			vecWeights.resize(_iSize);
			vecWI.resize(_iSize);
		}
	};

	struct tKeyFrame
	{
		fbxsdk::FbxAMatrix  matTransform;
		double				dTime;
	};

	struct tBone
	{
		std::string				strBoneName{};
		int					iDepth{};			// 계층구조 깊이
		int					iParentIndx{};	// 부모 Bone 의 인덱스
		fbxsdk::FbxAMatrix			matOffset{};		// Offset 행렬( -> 뿌리 -> Local)
		fbxsdk::FbxAMatrix			matBone{};
		std::vector<tKeyFrame>	vecKeyFrame{};
	};

	struct tAnimClip
	{
		std::string		strName{};
		fbxsdk::FbxTime		tStartTime{};
		fbxsdk::FbxTime		tEndTime{};
		
		fbxsdk::FbxLongLong	llTimeLength{};
		fbxsdk::FbxTime::EMode eMode{};
	};

	class FBXLoader :
		public Entity
	{
	public:
		FBXLoader();
		virtual ~FBXLoader();

	public:
		void Init();
		define::eResult LoadFbx(const std::filesystem::path& _strPath);

		// FbxMatrix -> Matrix
		static MATRIX GetMatrixFromFbxMatrix(fbxsdk::FbxAMatrix& _mat);

	public:
		int GetContainerCount() { return (int)mContainers.size(); }
		const tContainer& GetContainer(int _iIdx) { return mContainers[_iIdx]; }
		std::vector<tBone*>& GetBones() { return mBones; }
		std::vector<tAnimClip*>& GetAnimClip() { return mAnimClips; }



	private:
		void LoadMeshDataFromNode(fbxsdk::FbxNode* _pRoot);
		void LoadMesh(fbxsdk::FbxMesh* _pFbxMesh);
		void LoadMaterial(fbxsdk::FbxSurfaceMaterial* _pMtrlSur);

		void GetTangent(fbxsdk::FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
		void GetBinormal(fbxsdk::FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
		void GetNormal(fbxsdk::FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
		void GetUV(fbxsdk::FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);

		float4 GetMtrlData(fbxsdk::FbxSurfaceMaterial* _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName);
		std::string GetMtrlTextureName(fbxsdk::FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty);

		void LoadTexture();
		void CreateMaterial();

		// Animation
		void LoadSkeleton(fbxsdk::FbxNode* _pNode);
		void LoadSkeletonRecursive(fbxsdk::FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
		void LoadAnimationClip();
		void Triangulate(fbxsdk::FbxNode* _pNode);

		void LoadAnimationData(fbxsdk::FbxMesh* _pMesh, tContainer* _pContainer);
		void LoadWeightsAndIndices(fbxsdk::FbxCluster* _pCluster, int _iBoneIdx, tContainer* _pContainer);
		void LoadOffsetMatrix(fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer);
		void LoadKeyframeTransform(fbxsdk::FbxNode* _pNode, fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _matNodeTransform
			, int _iBoneIdx, tContainer* _pContainer);

		int FindBoneIndex(const std::string& _strBoneName);
		fbxsdk::FbxAMatrix GetTransform(fbxsdk::FbxNode* _pNode);

		void CheckWeightAndIndices(fbxsdk::FbxMesh* _pMesh, tContainer* _pContainer);

	private:
		fbxsdk::FbxManager*		mManager;
		fbxsdk::FbxScene*		mScene;
		fbxsdk::FbxImporter*	mImporter;

		std::vector<tContainer>				mContainers;

		// Animation
		std::vector<tBone*>					mBones;
		fbxsdk::FbxArray<fbxsdk::FbxString*>			mAnimNames;
		std::vector<tAnimClip*>				mAnimClips;
	};
}


