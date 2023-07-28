#include "EnginePCH.h"

#include "Material.h"
#include "ConstBuffer.h"

namespace mh
{
    using namespace mh::math;

    Material::Material()
        : IRes(eResourceType::Material)
        , mMode(eRenderingMode::Opaque)
    {

    }

    Material::~Material()
    {

    }

    HRESULT Material::Load(const std::filesystem::path& _path)
    {
        return E_NOTIMPL;
    }

    void Material::SetData(eGPUParam _param, void* _data)
    {
        /*switch (_param)
        {
        case mh::eGPUParam::Int:
            mCB.iData = *static_cast<int*>(_data);
            break;
        case mh::eGPUParam::Float:
            mCB.fData = *static_cast<float*>(_data);
            break;
        case mh::eGPUParam::Vector2:
            mCB.XY = *static_cast<Vector2*>(_data);
            break;
        case mh::eGPUParam::Vector3:
            mCB.XYZ = *static_cast<Vector3*>(_data);
            break;
        case mh::eGPUParam::Vector4:
            mCB.XYZW = *static_cast<Vector4*>(_data);
            break;
        case mh::eGPUParam::Matrix:
            mCB.Matrix = *static_cast<Matrix*>(_data);
            break;
        default:
            break;
        }*/

    }

    void Material::Bind()
    {
        for (size_t slotIndex = 0; slotIndex < (UINT)eTextureSlot::End; slotIndex++)
        {
            if (mTexture[slotIndex] == nullptr)
            {
				continue;
            }

            mTexture[slotIndex]->BindDataSRV((UINT)slotIndex, eShaderStageFlag::ALL);
        }

        if (mTexture[(UINT)eTextureSlot::Albedo])
        {
            mCB.UsedAlbedo = 1;
        }

        if (mTexture[(UINT)eTextureSlot::Normal])
        {
            mCB.UsedNormal = 1;
        }

        ConstBuffer* CB = RenderMgr::GetInst()->GetConstBuffer(eCBType::Material);
        CB->SetData(&mCB);

        eShaderStageFlag_ flag = eShaderStageFlag::VS | eShaderStageFlag::GS | eShaderStageFlag::PS;
        CB->BindData(flag);

        mShader->Binds();
    }

    void Material::Clear()
    {
        for (size_t slotIndex = 0; slotIndex < (UINT)eTextureSlot::End; slotIndex++)
        {
            if (mTexture[slotIndex] == nullptr)
			{
				continue;
			}

            mTexture[slotIndex]->UnBind();
        }
    }
}
