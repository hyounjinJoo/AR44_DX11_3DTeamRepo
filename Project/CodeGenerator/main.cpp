// CodeGenerator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "PCH_CodeGenerator.h"
#include "DirTree.h"

#include <Engine/define_Res.h>
#include <Engine/define_GPU.h>

//시작 지점 = $(SolutionDir) : 상대 경로로 작업해주면 된다.
int main(int argc, char* argv[])
{
    //Create variable name restraints regex
    define_Preset::Regex::g_VarForbiddenChars::CreateVarForbiddenRegex();

    constexpr const char* TexExt[] = { ".bmp", ".png", ".jpg", ".tga", ".dds" };
    constexpr const size_t TexExtSize = sizeof(TexExt) / sizeof(TexExt[0]);

    //Generate Texture Key
    {
        std::string regbase;
        regbase += R"((.+)\.()";

        for (size_t i = 0; i < TexExtSize; ++i)
        {
            std::string temp = TexExt[i];
            size_t pos = temp.find('.');
            if (std::string::npos != pos)
            {
                temp.erase(0, pos + 1);
            }
            regbase += temp;

            if (i == TexExtSize - 1ui64)
                regbase += ")$";
            else
                regbase += "|";
        }

        std::regex reg(regbase, std::regex::icase);

        DirTree DirTree;
        {
            stdfs::path DirPath = define_Preset::Path::Content::A;
            DirPath /= mh::define::strKey::GetResName(mh::define::eResourceType::Texture);
            DirTree.SearchRecursive(DirPath, reg);
        }

        stdfs::path outPath = define_Preset::Path::ContentsProj::A;
        outPath /= "strKey_Texture.h";
        DirTree.CreateStrKeyHeader(outPath, "Texture", false);
    }


    //Generate Compute Shader Key
    {
        std::regex reg(define_Preset::Regex::AllShader::A);

        DirTree DirTree;
        DirTree.SearchRecursive(define_Preset::Path::HLSL_Proj::A, reg);

        stdfs::path strKeyPath = define_Preset::Path::ContentsProj::A;
        strKeyPath /= "strKey_Shader.h";
        DirTree.CreateShaderStrKey(strKeyPath);


        //DirTree.CreateStrKeyHeader(DirPath / DIRECTORY_NAME, "Shader", true);

        //일단 미사용
        //DirTree.CreateCShaderCode(DirPath / define_Preset::Path::UserClassInit_CS::A);
    }

    //Generate Componets
    {
        std::regex regexCom(R"(Com_\w+\.h)");

        DirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ContentsProj::A;
        
        DirTree.SearchRecursive(DirPath, regexCom);

        DirTree.CreateStrKeyHeader(DirPath / "strKey_Component.h", "Com", true);
        
        tAddBaseClassDesc Desc = {};
        Desc.BaseType = "IComponent";
        Desc.IncludePCH = R"(#include "PCH_Contents.h")";
        Desc.IncludeMasterHeader = R"(#include "ContentsClassInitializer.h")";
        Desc.IncludeStrKeyHeaderName = R"(#include "strKey_Component.h")";
        Desc.IncludeManagerHeader = "#include <Engine/ComMgr.h>";
        Desc.MasterNamespace = "namespace mh";
        Desc.UsingNamespace = "using namespace mh::define;";
        Desc.Constructor_T_MacroDefine = R"(ComMgr::AddComConstructor<T>(strKey::Com::##T))";
        Desc.UserClassMgr_InitFuncName = "InitCom()";
        Desc._FilePath = DirPath / "ContentsClassInitializer_Component.cpp";

        DirTree.CreateComMgrInitCode(Desc);
    }

    //Generate Scipts
    {
        std::regex regexCom(R"(Script_\w+\.h)");

        DirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ContentsProj::A;

        DirTree.SearchRecursive(DirPath, regexCom);

        DirTree.CreateStrKeyHeader(DirPath / "strKey_Script.h", "Script", true);

        tAddBaseClassDesc Desc = {};
        Desc.BaseType = "IScript";
        Desc.IncludePCH = R"(#include "PCH_Contents.h")";
        Desc.IncludeMasterHeader = R"(#include "ContentsClassInitializer.h")";
        Desc.IncludeStrKeyHeaderName = R"(#include "strKey_Script.h")";
        Desc.IncludeManagerHeader = "#include <Engine/ComMgr.h>";
        Desc.MasterNamespace = "namespace mh";
        Desc.UsingNamespace = "using namespace mh::define;";
        Desc.Constructor_T_MacroDefine = R"(ComMgr::AddComConstructor<T>(strKey::Script::##T))";
        Desc.UserClassMgr_InitFuncName = "InitScript()";
        Desc._FilePath = DirPath / "ContentsClassInitializer_Script.cpp";

        DirTree.CreateComMgrInitCode(Desc);
    }
    return 0;
}
