#include "Material.h"

namespace mh::graphics
{
    using namespace mh::math;

    Material::Material()
        : Resource(eResourceType::Material)
        , mMode(eRenderingMode::Opaque)
    {

    }

    Material::~Material()
    {

    }

    HRESULT Material::Load(const std::wstring& _path)
    {
        return E_NOTIMPL;
    }

    void Material::SetData(eGPUParam _param, void* _data)
    {
        switch (_param)
        {
        case mh::graphics::eGPUParam::Int:
            mCB.iData = *static_cast<int*>(_data);
            break;
        case mh::graphics::eGPUParam::Float:
            mCB.fData = *static_cast<float*>(_data);
            break;
        case mh::graphics::eGPUParam::Vector2:
            mCB.XY = *static_cast<Vector2*>(_data);
            break;
        case mh::graphics::eGPUParam::Vector3:
            mCB.XYZ = *static_cast<Vector3*>(_data);
            break;
        case mh::graphics::eGPUParam::Vector4:
            mCB.XYZW = *static_cast<Vector4*>(_data);
            break;
        case mh::graphics::eGPUParam::Matrix:
            mCB.Matrix = *static_cast<Matrix*>(_data);
            break;
        default:
            break;
        }

    }

    void Material::Bind()
    {
        for (size_t slotIndex = 0; slotIndex < (UINT)eTextureSlot::End; slotIndex++)
        {
            if (mTexture[slotIndex] == nullptr)
            {
				continue;
            }

            mTexture[slotIndex]->BindShaderResource(eShaderStage::VS, slotIndex);
            mTexture[slotIndex]->BindShaderResource(eShaderStage::HS, slotIndex);
            mTexture[slotIndex]->BindShaderResource(eShaderStage::DS, slotIndex);
            mTexture[slotIndex]->BindShaderResource(eShaderStage::GS, slotIndex);
            mTexture[slotIndex]->BindShaderResource(eShaderStage::PS, slotIndex);
            mTexture[slotIndex]->BindShaderResource(eShaderStage::CS, slotIndex);
        }

        ConstantBuffer* CB = renderer::constantBuffers[(UINT)eCBType::Material];
        CB->SetData(&mCB);
        CB->Bind(eShaderStage::VS);
        CB->Bind(eShaderStage::GS);
        CB->Bind(eShaderStage::PS);

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

            mTexture[slotIndex]->Clear();
        }
    }
}