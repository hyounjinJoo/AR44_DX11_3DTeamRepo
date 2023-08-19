#include "PCH_Engine.h"
#include "Skeleton.h"

#include "define_Util.h"

using namespace mh::define;
namespace mh
{
	Skeleton::Skeleton()
		: m_vecBones{}
		, m_pBoneOffset{}
		, m_vecAnimClip{}
		, m_pBoneFrameData{}
	{
	}

	Skeleton::~Skeleton()
	{
	}

	define::eResult Skeleton::Save(const std::filesystem::path& _fullPath)
	{
		//// Animation3D 정보
		//std::vector<define::tMTAnimClip>		m_vecAnimClip;
		Binary::SaveValue(ofs, m_vecAnimClip.size());
		for (size_t i = 0; i < m_vecAnimClip.size(); ++i)
		{
			Binary::SaveStr(ofs, m_vecAnimClip[i].strAnimName);
			Binary::SaveValue(ofs, m_vecAnimClip[i].Val);
		}


		//std::vector<define::tMTBone>			m_vecBones;
		Binary::SaveValue(ofs, m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			Binary::SaveStr(ofs, m_vecBones[i].strBoneName);
			Binary::SaveValue(ofs, m_vecBones[i].Val);
			Binary::SaveValueVector(ofs, m_vecBones[i].vecKeyFrame);
		}

		return define::eResult();
	}
	define::eResult Skeleton::Load(const std::filesystem::path& _fullPath)
	{

		//// Animation3D 정보
//std::vector<define::tMTAnimClip>		m_vecAnimClip;
		{
			size_t size{};
			Binary::LoadValue(ifs, size);
			m_vecAnimClip.resize(size);
			for (size_t i = 0; i < size; ++i)
			{
				Binary::LoadStr(ifs, m_vecAnimClip[i].strAnimName);
				Binary::LoadValue(ifs, m_vecAnimClip[i].Val);
			}
		}

		//std::vector<define::tMTBone>			m_vecBones;
				//std::vector<define::tMTBone>			m_vecBones;
		{
			size_t size{};
			Binary::LoadValue(ifs, size);
			m_vecBones.resize(size);
			for (size_t i = 0; i < size; ++i)
			{
				Binary::LoadStr(ifs, m_vecBones[i].strBoneName);
				Binary::LoadValue(ifs, m_vecBones[i].Val);
				Binary::LoadValueVector(ifs, m_vecBones[i].vecKeyFrame);
			}
		}

		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		if (IsAnimMesh())
		{
			// BoneOffet 행렬
			std::vector<MATRIX> vecOffset;
			std::vector<tFrameTranslation> vecFrameTrans;

			//프레임 갯수 구하기
			size_t iFrameCount = 0;
			for (size_t i = 0; i < m_vecBones.size(); ++i)
			{
				iFrameCount = max(iFrameCount, (size_t)m_vecBones[i].vecKeyFrame.size());
			}

			vecFrameTrans.resize((UINT)m_vecBones.size() * iFrameCount);
			for (size_t i = 0; i < m_vecBones.size(); ++i)
			{
				vecOffset.push_back(m_vecBones[i].Val.matOffset);

				for (size_t j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
				{
					vecFrameTrans[(UINT)m_vecBones.size() * j + i]
						= m_vecBones[i].vecKeyFrame[j].FrameTrans;
				}
			}


			//Create
			m_pBoneOffset = std::make_unique<StructBuffer>();
			tSBufferDesc Desc{};
			Desc.eSBufferType = eStructBufferType::READ_ONLY;
			Desc.REGISLOT_t_SRV = Register_t_g_arrBoneMat;
			m_pBoneOffset->SetDesc(Desc);
			m_pBoneOffset->Create<MATRIX>(vecOffset.size(), vecOffset.data(), vecOffset.size());


			m_pBoneFrameData = std::make_unique<StructBuffer>();
			Desc = tSBufferDesc{};
			Desc.REGISLOT_t_SRV = Register_t_g_arrFrameTrans;
			Desc.eSBufferType = eStructBufferType::READ_ONLY;
			m_pBoneFrameData->SetDesc(Desc);
			size_t SbufferSize = vecOffset.size() * (size_t)iFrameCount;
			m_pBoneFrameData->Create<tFrameTranslation>(SbufferSize, vecFrameTrans.data(), SbufferSize);
		}
		//StructBuffer* m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
		//StructBuffer* m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)


		return define::eResult();
	}

	define::eResult Skeleton::CreateFromFBX(FBXLoader* _pFBXLoader)
	{
		std::vector<tBone*>& vecBone = _loader.GetBones();
		UINT iFrameCount = 0;
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			tMTBone bone = {};
			bone.Val.iDepth = vecBone[i]->iDepth;
			bone.Val.iParentIndx = vecBone[i]->iParentIndx;
			bone.Val.matBone = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i]->matBone);
			bone.Val.matOffset = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i]->matOffset);
			bone.strBoneName = vecBone[i]->strBoneName;

			for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
			{
				tMTKeyFrame Keyframe = {};
				Keyframe.dTime = vecBone[i]->vecKeyFrame[j].dTime;
				Keyframe.iFrame = j;

				const auto& keyFrameTransform = vecBone[i]->vecKeyFrame[j].matTransform.GetT();
				Keyframe.FrameTrans.vTranslate.x = (float)keyFrameTransform.mData[0];
				Keyframe.FrameTrans.vTranslate.y = (float)keyFrameTransform.mData[1];
				Keyframe.FrameTrans.vTranslate.z = (float)keyFrameTransform.mData[2];
				Keyframe.FrameTrans.vTranslate.w = (float)keyFrameTransform.mData[3];
				//float4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f),

				const auto& keyFrameScale = vecBone[i]->vecKeyFrame[j].matTransform.GetS();
				Keyframe.FrameTrans.vScale.x = (float)keyFrameScale.mData[0];
				Keyframe.FrameTrans.vScale.y = (float)keyFrameScale.mData[1];
				Keyframe.FrameTrans.vScale.z = (float)keyFrameScale.mData[2];
				Keyframe.FrameTrans.vScale.w = (float)keyFrameScale.mData[3];
				//float4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale, 0.f),

				const auto& keyFrameQuat = vecBone[i]->vecKeyFrame[j].matTransform.GetQ();
				Keyframe.FrameTrans.qRot.x = (float)keyFrameQuat.mData[0];
				Keyframe.FrameTrans.qRot.y = (float)keyFrameQuat.mData[1];
				Keyframe.FrameTrans.qRot.z = (float)keyFrameQuat.mData[2];
				Keyframe.FrameTrans.qRot.w = (float)keyFrameQuat.mData[3];

				bone.vecKeyFrame.push_back(Keyframe);
			}

			iFrameCount = max(iFrameCount, (UINT)bone.vecKeyFrame.size());

			pMesh->m_vecBones.push_back(bone);
		}

		std::vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

		for (UINT i = 0; i < vecAnimClip.size(); ++i)
		{
			tMTAnimClip tClip = {};

			tClip.strAnimName = vecAnimClip[i]->strName;
			tClip.Val.dStartTime = vecAnimClip[i]->tStartTime.GetSecondDouble();
			tClip.Val.dEndTime = vecAnimClip[i]->tEndTime.GetSecondDouble();
			tClip.Val.dTimeLength = tClip.Val.dEndTime - tClip.Val.dStartTime;

			tClip.Val.iStartFrame = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.Val.iEndFrame = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.Val.iFrameLength = tClip.Val.iEndFrame - tClip.Val.iStartFrame;
			tClip.Val.eMode = vecAnimClip[i]->eMode;

			pMesh->m_vecAnimClip.push_back(tClip);
		}

		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		if (pMesh->IsAnimMesh())
		{
			// BoneOffet 행렬
			std::vector<MATRIX> vecOffset;
			std::vector<tFrameTranslation> vecFrameTrans;
			vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

			for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
			{
				vecOffset.push_back(pMesh->m_vecBones[i].Val.matOffset);

				for (size_t j = 0; j < pMesh->m_vecBones[i].vecKeyFrame.size(); ++j)
				{
					vecFrameTrans[(UINT)pMesh->m_vecBones.size() * j + i]
						= pMesh->m_vecBones[i].vecKeyFrame[j].FrameTrans;
				}
			}

			//Create
			pMesh->m_pBoneOffset = std::make_unique<StructBuffer>();
			tSBufferDesc Desc{};
			Desc.eSBufferType = eStructBufferType::READ_ONLY;
			Desc.REGISLOT_t_SRV = Register_t_g_arrBoneMat;
			pMesh->m_pBoneOffset->SetDesc(Desc);
			pMesh->m_pBoneOffset->Create<MATRIX>(vecOffset.size(), vecOffset.data(), vecOffset.size());


			pMesh->m_pBoneFrameData = std::make_unique<StructBuffer>();
			Desc = tSBufferDesc{};
			Desc.REGISLOT_t_SRV = Register_t_g_arrFrameTrans;
			Desc.eSBufferType = eStructBufferType::READ_ONLY;
			pMesh->m_pBoneFrameData->SetDesc(Desc);
			size_t SbufferSize = vecOffset.size() * (size_t)iFrameCount;
			pMesh->m_pBoneFrameData->Create<tFrameTranslation>(SbufferSize, vecFrameTrans.data(), SbufferSize);
		}




		return define::eResult();
	}
}

