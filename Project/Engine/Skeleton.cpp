#include "PCH_Engine.h"
#include "Skeleton.h"

#include "define_Util.h"
#include "DefaultShader/SH_Resource.hlsli"
#include "StructBuffer.h"
#include "FBXLoader.h"
#include "PathMgr.h"
#include "Animation3D.h"

namespace mh
{
	using namespace mh::define;
	Skeleton::Skeleton()
		: m_vecBones{}
		, m_pBoneOffset{}
		, mMapAnimations{}
		, mAnimationMaxFrameSize()
	{
	}

	Skeleton::~Skeleton()
	{
		for (auto& iter : mMapAnimations)
		{
			if (iter.second)
				delete iter.second;
		}
	}

	eResult Skeleton::Save(const std::filesystem::path& _fileName)
	{
		std::fs::path fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		fullPath /= _fileName;
		if (false == std::fs::exists(fullPath.parent_path()))
		{
			std::fs::create_directories(fullPath);
		}
		fullPath.replace_extension(define::strKey::Ext_Skeleton);

		std::ofstream ofs(fullPath, std::ios::binary);
		if (false == ofs.is_open())
		{
			ERROR_MESSAGE_W(L"Bone 저장에 실패했습니다.");
			return eResult::Fail_OpenFile;
		}

		Binary::SaveValue(ofs, m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			Binary::SaveStr(ofs, m_vecBones[i].strBoneName);
			Binary::SaveValue(ofs, m_vecBones[i].Val);
			Binary::SaveValueVector(ofs, m_vecBones[i].vecKeyFrame);
		}

		return eResult::Success;
	}
	eResult Skeleton::Load(const std::filesystem::path& _fileName)
	{
		std::fs::path fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		fullPath /= _fileName;
		if (false == std::fs::exists(fullPath.parent_path()))
		{
			std::fs::create_directories(fullPath);
		}
		fullPath.replace_extension(define::strKey::Ext_Skeleton);

		std::ifstream ifs(fullPath, std::ios::binary);
		if (false == ifs.is_open())
		{
			ERROR_MESSAGE_W(L"Bone 저장에 실패했습니다.");
			return eResult::Fail_OpenFile;
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

		CreateBoneOffsetSBuffer();

		//StructBuffer* m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
		//StructBuffer* m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)


		return eResult::Success;
	}

	eResult Skeleton::CreateFromFBX(FBXLoader* _fbxLoader)
	{
		const std::vector<tFBXBone>& vecBone = _fbxLoader->GetBones();
		if (vecBone.empty())
		{
			return eResult::Fail_Empty;
		}

		UINT iFrameCount = 0;
		m_vecBones.reserve(vecBone.size());
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			m_vecBones.push_back(tMTBone{});
			tMTBone& bone = m_vecBones.back();

			bone.Val.iDepth = vecBone[i].Depth;
			bone.Val.iParentIndx = vecBone[i].ParentIndx;
			bone.Val.matBone = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i].matBone);
			bone.Val.matOffset = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i].matOffset);
			bone.strBoneName = vecBone[i].strBoneName;
		}
		CreateBoneOffsetSBuffer();

		const auto& animClip = _fbxLoader->GetAnimations();
		for (size_t i = 0; i < animClip.size(); ++i)
		{
			std::unique_ptr<Animation3D> anim = std::make_unique<Animation3D>();

			eResult result = anim->LoadFromFBX(this, &animClip[i]);
			if (eResultFail(result))
			{
				ERROR_MESSAGE_W(L"애니메이션 생성 실패");
				return result;
			}
			
			std::string animName = anim->GetKey();
			if (animName.empty())
			{
				//애니메이션이 1000개를 넘을거같진 않으니 3자리까지만 고정
				size_t numAnim = animClip.size();
				int digits = 3;
				do
				{
					numAnim /= (size_t)10;
					--digits;
				} while (numAnim);

				if (digits < 0)
					digits = 0;

				animName = "Anim_";
				for (int i = 0; i < digits; ++i)
				{
					animName += "0";
				}
				
				animName += std::to_string(i);
			}

			mMapAnimations.insert(std::make_pair(animName, anim.release()));
		}

		return eResult::Success;
	}

	void Skeleton::CreateBoneOffsetSBuffer()
	{
		// BoneOffet 행렬
		std::vector<MATRIX> vecOffset;
		vecOffset.reserve(m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].Val.matOffset);
		}

		//Create
		m_pBoneOffset = std::make_unique<StructBuffer>();
		tSBufferDesc Desc{};
		Desc.eSBufferType = eStructBufferType::READ_ONLY;
		Desc.REGISLOT_t_SRV = Register_t_g_OffsetArray;
		m_pBoneOffset->SetDesc(Desc);
		m_pBoneOffset->Create<MATRIX>(vecOffset.size(), vecOffset.data(), vecOffset.size());
	}

	const Animation3D* Skeleton::FindAnimation(const std::string& _strAnimName)
	{
		Animation3D* retPtr = nullptr;
		const auto& iter = mMapAnimations.find(_strAnimName);
		if (iter != mMapAnimations.end())
		{
			retPtr = iter->second;
		}
		return retPtr;
	}
}

