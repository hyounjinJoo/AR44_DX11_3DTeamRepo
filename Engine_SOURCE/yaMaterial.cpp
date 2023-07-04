#include "yaMaterial.h"

namespace ya::graphics
{
    Material::Material()
        : Resource(eResourceType::Material)
        , mMode(eRenderingMode::Opaque)
    {

    }

    Material::~Material()
    {

    }

    HRESULT Material::Load(const std::wstring& path)
    {
        
        
        
        return E_NOTIMPL;
    }

    void Material::SetData(eGPUParam param, void* data)
    {
        switch (param)
        {
        case ya::graphics::eGPUParam::Int:
            mCB.iData = *static_cast<int*>(data);
            break;
        case ya::graphics::eGPUParam::Float:
            mCB.fData = *static_cast<float*>(data);
            break;
        case ya::graphics::eGPUParam::Vector2:
            mCB.xy = *static_cast<Vector2*>(data);
            break;
        case ya::graphics::eGPUParam::Vector3:
            mCB.xyz = *static_cast<Vector3*>(data);
            break;
        case ya::graphics::eGPUParam::Vector4:
            mCB.xyzw = *static_cast<Vector4*>(data);
            break;
        case ya::graphics::eGPUParam::Matrix:
            mCB.matrix = *static_cast<Matrix*>(data);
            break;
        default:
            break;
        }

    }

    void Material::Bind()
    {
        for (size_t i = 0; i < (UINT)eTextureSlot::End; i++)
        {
            if (mTexture[i] == nullptr)
                continue;

            mTexture[i]->BindShaderResource(eShaderStage::VS, i);
            mTexture[i]->BindShaderResource(eShaderStage::HS, i);
            mTexture[i]->BindShaderResource(eShaderStage::DS, i);
            mTexture[i]->BindShaderResource(eShaderStage::GS, i);
            mTexture[i]->BindShaderResource(eShaderStage::PS, i);
            mTexture[i]->BindShaderResource(eShaderStage::CS, i);
        }

        ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Material];
        pCB->SetData(&mCB);
        pCB->Bind(eShaderStage::VS);
        pCB->Bind(eShaderStage::GS);
        pCB->Bind(eShaderStage::PS);

        mShader->Binds();
    }

    void Material::Clear()
    {
        for (size_t i = 0; i < (UINT)eTextureSlot::End; i++)
        {
            if (mTexture[i] == nullptr)
                continue;

            mTexture[i]->Clear();
        }
    }
}