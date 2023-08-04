#include "PCH.h"
#include "DirTree.h"

#include "define_Util.h"
#include "json-cpp/json.h"
#include "CodeWriter.h"
#include "DirTreeNode.h"

#include <Engine/define_Res.h>
#include <Engine/GraphicsShader.h>
#include <Engine/ComputeShader.h>

DirTree::DirTree()
	: m_RootDir()
{
}

DirTree::~DirTree()
{
}

HRESULT DirTree::SearchRecursive(stdfs::path const& _RootPath, std::regex const& _regex)
{
	m_RootDir.Clear();

	return m_RootDir.SearchRecursive( _RootPath, _RootPath, _regex);
}


HRESULT DirTree::CreateStrKeyHeader(stdfs::path const& _FilePath, stdfs::path const& _RootNamespace, bool _bEraseExtension)
{
	CodeWriter Writer;

	Writer.WriteCode(0, define_Preset::Keyword::Head::A);

	Writer.WriteCode(0, define_Preset::Keyword::DefineSTRKEY::A);

	//루트 노드의 경우 이름도 알맞게 바꿔줘야 함.
	Writer.WriteCode(0);
	Writer.WriteCode(0);

	std::string strCode = "namespace mh::define::strKey::";
	strCode += _RootNamespace.string();
	Writer.WriteCode(0, strCode);

	HRESULT hr = m_RootDir.WriteStrKeyTree(Writer, _bEraseExtension);

	if (FAILED(hr))
		return hr;

	return Writer.SaveAll<char>(_FilePath);
}




HRESULT DirTree::CreateComMgrInitCode(tAddBaseClassDesc const& _Desc)
{
	CodeWriter Writer;

	Writer.WriteCode(0, define_Preset::Keyword::Head::A);


	{
		Writer.WriteCode(0, _Desc.IncludePCH);
		Writer.WriteCode(0);
		Writer.WriteCode(0, _Desc.IncludeMasterHeader);
		Writer.WriteCode(0, _Desc.IncludeManagerHeader);
	}
	{
		Writer.WriteCode(0, _Desc.IncludeStrKeyHeaderName);
	}

	{
		Writer.WriteCode(0, _Desc.IncludeBaseTypeHeader);
		Writer.WriteCode(0);
	}

	//1번 버퍼에 CS 생성 코드를 작성
	{
		Writer.WriteCode(1, "");

		{
			//CONSTRUCTOR_T(T) 
			std::string strCode = define_Preset::Keyword::Define_Constructor_T::A;
			strCode += _Desc.Constructor_T_MacroDefine;
			Writer.WriteCode(1, strCode);

			Writer.WriteCode(1, "");
		}

		{
			Writer.WriteCode(1, _Desc.MasterNamespace);
			Writer.OpenBracket(1);
			Writer.WriteCode(1, _Desc.UsingNamespace);
		}

		std::string strCode = "void ContentsClassInitializer::";
		strCode += _Desc.UserClassMgr_InitFuncName;

		Writer.WriteCode(1, strCode);
		Writer.OpenBracket(1);
		//Writer.WriteCode("cUserClassMgr* pMgr = cUserClassMgr::GetInst();", 1);
	}

	std::vector<stdfs::path> vecCSFilePath;
	//노드를 순회돌면서 이름을 정리시킨다.
	m_RootDir.GetAllFiles(vecCSFilePath, false);

	//순회를 돌면서 각 버퍼에 코드 작성
	//0번 버퍼: include
	//1번 버퍼: 클래스 생성
	for (size_t i = 0; i < vecCSFilePath.size(); ++i)
	{
		//0번 버퍼에 include 작성
		{
			const std::string& FileName = vecCSFilePath[i].filename().string();

			std::string strCode;
			strCode += define_Preset::Keyword::IncludeBegin::A;
			strCode += FileName + ".h";
			strCode += "\"";
			Writer.WriteCode(0, strCode);
		}

		//1번 버퍼에 클래스 생성 코드 작성
		{
			const std::string& ClassName = vecCSFilePath[i].filename().replace_extension("").string();

			std::string strCode;
			strCode += define_Preset::Keyword::Constructor_T::A;
			strCode += ClassName;
			strCode += ");";
			Writer.WriteCode(1, strCode);
		}
	}


	Writer.CloseBracket(1, false);
	Writer.CloseBracket(1, false);

	HRESULT hr = Writer.SaveAll<char>(_Desc._FilePath);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	return S_OK;
}





HRESULT DirTree::CreateShaderStrKey(stdfs::path const& _FilePath)
{
	CodeWriter Writer;

	std::vector<stdfs::path> vecShaderFileName;
	HRESULT hr = m_RootDir.GetAllFiles(vecShaderFileName, false);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	std::unordered_map<stdfs::path, tShaderGroup> umapGSGroup;
	std::vector<stdfs::path> vecCS;

	std::regex CSRegex(define_Preset::Regex::CShaderRegex::A);
	//파일 순회돌면서 그래픽 쉐이더 파일 정리
	for (size_t i = 0; i < vecShaderFileName.size(); ++i)
	{
		//std::string으로 변경
		std::string strFileName = vecShaderFileName[i].string();

		bool bIsGS = false;
		for (int j = 0; j < (int)mh::define::eGSStage::END; ++j)
		{
			size_t Pos = strFileName.find(mh::define::strKey::ArrGSPrefix[j]);
			if (std::string::npos != Pos)
			{
				//쉐이더별로 지정한 접두사가 있을 경우 해당 접두사를 제거한다.
				strFileName.erase(0, Pos + strlen(mh::define::strKey::ArrGSPrefix[j]));

				//파일의 확장자를 json으로 변경하고, 키값으로 사용.
				stdfs::path ShaderGroupName = strFileName;
				ShaderGroupName.replace_extension(mh::define::strKey::Ext_ShaderSetting);

				//쉐이더 그룹명에 각 쉐이더 파이프라인의 데이터를 담은 파일의 이름을 추가한다.
				umapGSGroup[ShaderGroupName].FileName[j] = vecShaderFileName[i];

				bIsGS = true;

				break;
			}
		}

		//그래픽 쉐이더가 아닐 경우 컴퓨트쉐이더인지 검사
		if (false == bIsGS)
		{
			//컴퓨트쉐이더일 경우 컴퓨트쉐이더 벡터에 값을 추가
			if(std::regex_search(strFileName, CSRegex))
			{
				vecCS.push_back(vecShaderFileName[i].replace_extension());
			}
		}
	}


	//코드 작성 시작
	Writer.WriteCode(0, define_Preset::Keyword::Head::A);
	Writer.WriteCode(0, define_Preset::Keyword::DefineSTRKEY::A);

	Writer.WriteCode(0);
	Writer.WriteCode(0);

	{
		std::string strCode;
		strCode +=  "namespace ";
		strCode += define_Preset::Keyword::strKey::A;
		strCode += "Shader";
		Writer.WriteCode(0, strCode);
		Writer.OpenBracket(0);
	}

	{
		std::string strCode;
		strCode += "namespace Graphics";
		Writer.WriteCode(0, strCode);
		Writer.OpenBracket(0);
	}

	for (const auto& iter : umapGSGroup)
	{
		std::string strCode;
		strCode +=  "STRKEY ";
		strCode += iter.first.filename().replace_extension("").string();
		strCode += " = \"";
		strCode += iter.first.string();
		strCode += "\";";
		Writer.WriteCode(0, strCode);
	}
	Writer.CloseBracket(0, true);

	{
		Writer.AddIndentation(1);
		std::string strCode;
		strCode += "namespace Compute";
		Writer.WriteCode(1, strCode);
		Writer.OpenBracket(1);
	}

	for (size_t i = 0; i < vecCS.size(); ++i)
	{
		std::string strCode;
		strCode += "STRKEY ";
		strCode += vecCS[i].filename().replace_extension("").string();
		strCode += " = \"";
		strCode += vecCS[i].filename().replace_extension(".cso").string();
		strCode += "\";";
		Writer.WriteCode(1, strCode);
	}
	Writer.CloseBracket(1, true);
	Writer.CloseBracket(1);

	//Writer.CloseBracketAll(0);

	return Writer.SaveAll<char>(_FilePath);
}


namespace mh
{
	using namespace mh::define;

	eResult SaveJson(Json::Value* _pjVal)
	{
		return eResult();
	}

	eResult LoadJson(const Json::Value* _pjVal)
	{
		return eResult();
	}
}
