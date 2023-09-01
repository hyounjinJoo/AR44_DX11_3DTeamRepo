#include "PCH_Engine.h"

#include "Material.h"
#include "ConstBuffer.h"

#include "json-cpp/json.h"
#include "ResMgr.h"
#include "PathMgr.h"

#include "json-cpp/json.h"

namespace mh
{
    using namespace mh;

    Material::Material()
        : IRes(eResourceType::Material)
        , mCB{}
        , mMode(eRenderingMode::Opaque)
        , mShader{}
        , mTextures{}
    {
    }


    Material::Material(const Material& _other)
        : IRes(_other)
        , mShader(_other.mShader)
        , mTextures(_other.mTextures)
        , mCB(_other.mCB)
        , mMode(_other.mMode)
    {
    }

    Material::~Material()
    {
    }

    eResult Material::Save(const std::fs::path& _filePath, const std::fs::path& _basePath)
    {
        IRes::Save(_filePath, _basePath);
        std::fs::path fullPath = PathMgr::CreateFullPathToContent(_filePath, _basePath, GetResType());
        fullPath.replace_extension(strKey::Ext_Material);


        std::ofstream ofs(fullPath);
        if (false == ofs.is_open())
        {
            ERROR_MESSAGE_W(L"파일 열기에 실패했습니다.");
            return eResult::Fail_OpenFile;
        }
            

        Json::Value jVal{};
        eResult result = SaveJson(&jVal);
        if (eResultFail(result))
            return result;

        ofs << jVal;
        ofs.close();

        return eResult::Success;
    }

    eResult Material::Load(const std::fs::path& _filePath, const std::fs::path& _basePath)
    {
        IRes::Load(_filePath, _basePath);
        std::fs::path fullPath = PathMgr::CreateFullPathToContent(_filePath, _basePath, GetResType());
        fullPath.replace_extension(strKey::Ext_Material);
        if (false == std::fs::exists(fullPath))
        {
            return eResult::Fail_OpenFile;
        }

        std::ifstream ifs(fullPath);
        if (false == ifs.is_open())
        {
            ERROR_MESSAGE_W(L"Material 파일이 없습니다.");
            return eResult::Fail_OpenFile;
        }

        Json::Value jVal{};
        ifs >> jVal;
        ifs.close();

        eResult result = LoadJson(&jVal);
        if (eResultFail(result))
            return result;


        return eResult::Success;
    }

    eResult Material::SaveJson(Json::Value* _pJVal)
    {
        //nullptr 확인
        if (nullptr == _pJVal)
        {
            return eResult::Fail_Nullptr;
        }

        //부모 클래스 저장
        eResult result = IRes::SaveJson(_pJVal);
        if (eResultFail(result))
        {
            return result;
        }

        //레퍼런스로 바꾸고
        Json::Value& jVal = *_pJVal;

        //단순 Value의 경우에는 매크로로 바로 저장 가능
        Json::MH::SaveValue(_pJVal, JSON_KEY_PAIR(mCB));
        Json::MH::SaveValue(_pJVal, JSON_KEY_PAIR(mMode));


        //포인터의 경우에는 포인터 자체를 저장하는게 아니라 Key를 저장
        if (mShader)
        {
            jVal[JSON_KEY(mShader)] = mShader->GetKey();
        }
        Json::MH::SavePtrStrKeyVector(_pJVal, JSON_KEY_PAIR(mTextures));

        return eResult::Success;
    }

    eResult Material::LoadJson(const Json::Value* _pJVal)
    {
        //nullptr 확인
        if (nullptr == _pJVal)
        {
            return eResult::Fail_Nullptr;
        }

        //부모 클래스 저장
        eResult result = IRes::LoadJson(_pJVal);
        if (eResultFail(result))
        {
            return result;
        }
        const Json::Value& jVal = *_pJVal;

        //단순 Value의 경우에는 함수로 바로 불러오기 가능
        Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mMode));
        Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mCB));

        //쉐이더 데이터가 있는지 확인하고 가져온 키값으로 쉐이더를 로드
        std::string shaderStrKey = Json::MH::LoadPtrStrKey(_pJVal, JSON_KEY_PAIR(mShader));
        if (false == shaderStrKey.empty())
        {
            //쉐이더는 Base Path를 사용하지 않는다
            SetShader(ResMgr::Load<GraphicsShader>(shaderStrKey));
        }
        
        //포인터 배열은 MH::LoadPtrStrKeyVector 함수를 통해서 Key값을 싹 받아올 수 있음.
        const auto& vecLoad = Json::MH::LoadPtrStrKeyVector(_pJVal, JSON_KEY_PAIR(mTextures));
        for (size_t i = 0; i < vecLoad.size(); ++i)
        {
            if (false == vecLoad[i].empty())
            {
                SetTexture((eTextureSlot)i, ResMgr::Load<Texture>(vecLoad[i], GetBasePath()));
            }
        }
        return eResult::Success;
    }


    void Material::BindData()
    {
        for (size_t slotIndex = 0; slotIndex < (uint)eTextureSlot::END; slotIndex++)
        {
            if (mTextures[slotIndex])
            {
                mTextures[slotIndex]->BindDataSRV((uint)slotIndex, eShaderStageFlag::ALL);
            }
            else
            {
                Texture::ClearSRV((UINT)slotIndex);
            }
        }

        static ConstBuffer* CB = RenderMgr::GetConstBuffer(eCBType::Material);
        CB->SetData(&mCB);

        eShaderStageFlag_ flag = eShaderStageFlag::VS | eShaderStageFlag::GS | eShaderStageFlag::PS;
        CB->BindData(flag);

        mShader->BindData();
    }

    void Material::UnBindData()
    {
        for (size_t slotIndex = 0; slotIndex < (uint)eTextureSlot::END; slotIndex++)
        {
            if (mTextures[slotIndex])
                mTextures[slotIndex]->UnBindData();
        }
    }
}
