#include "PCH.h"

#include "DirTreeNode.h"
#include "define_Util.h"
#include "define_CodeGen.h"
#include "CodeWriter.h"

DirTreeNode::DirTreeNode()
	: m_pParent()
	, m_vecChild()
	, m_DirName()
	, m_vecFileName()
{
}

DirTreeNode::DirTreeNode(DirTreeNode* _pParent)
	: m_pParent(_pParent)
	, m_vecChild()
	, m_DirName()
	, m_vecFileName()
{
	assert(_pParent);
}

DirTreeNode::~DirTreeNode()
{
	size_t size = m_vecChild.size();
	for (size_t i = (size_t)0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
}

void DirTreeNode::Clear()
{
	size_t size = m_vecChild.size();
	for (size_t i = (size_t)0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
	m_vecChild.clear();
	m_DirName.clear();
	m_pParent = nullptr;
}

HRESULT DirTreeNode::SearchRecursive(stdfs::path const& _rootPath, stdfs::path const& _path, std::regex const& _regex)
{
	//들어온 Path 자체가 폴더 경로가 아닐 경우에는 실패 반환
	if (false == stdfs::is_directory(_path))
		return E_INVALIDARG;

	//디렉토리 이름을 등록
	if(false == IsRoot())
		m_DirName = _path.lexically_relative(_rootPath);

	try
	{
		for (const auto& dirIter : stdfs::directory_iterator(_path))
		{
			//포함 항목 검사
			
			//파일명일 경우 - 확장자 및 파일명을 확인하고, 일치하는 경우에만 파일명을 등록
			if (false == dirIter.is_directory())
			{
				stdfs::path const& filename = dirIter.path().filename();
				//확장자의 경우 정확히 일치하는지 확인
				if (std::regex_search(filename.string(), _regex))
				{
					m_vecFileName.push_back(filename);
				}
			}

			//폴더명일 경우
			else
			{
				//폴더를 발견했을 경우 새 노드를 생성 후 재귀호출
				DirTreeNode* pNode = new DirTreeNode(this);
				HRESULT hr = pNode->SearchRecursive(_rootPath, dirIter.path(), _regex);
				
				if (ERROR_EMPTY == hr)
				{
					SAFE_DELETE(pNode);
					continue;
				}
				else if (FAILED(hr))
				{
					SAFE_DELETE(pNode);
					return hr;
				}


				AddChild(pNode);
			}
		}

		//순회를 다 돌았는데 자식 노드도 없고 자신에게 파일도 안들어 있을경우 -> ERROR_EMPTY 반환
		//ERROR_EMPTY가 반환되면 해당 노드가 제거됨.
		if (IsLeaf() && m_vecFileName.empty())
			return ERROR_EMPTY;
	}
	catch (stdfs::filesystem_error const& _err)
	{
		ERROR_MESSAGE_A(_err.what());
		return E_ACCESSDENIED;
	}

	return S_OK;
}



HRESULT DirTreeNode::GetAllFiles(__out std::vector<stdfs::path>& _vecFile, bool _bAddRelativeDir)
{
	_vecFile.clear();

	for (size_t i = 0; i < m_vecFileName.size(); ++i)
	{
		if (IsRoot() || _bAddRelativeDir)
		{
			_vecFile.push_back(m_vecFileName[i]);
		}
		else
		{
			_vecFile.push_back(m_DirName / m_vecFileName[i]);
		}
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		HRESULT hr = m_vecChild[i]->GetAllFiles(_vecFile, _bAddRelativeDir);
		if (FAILED(hr))
		{
			DEBUG_BREAK;
			return hr;
		}
	}

	return S_OK;
}


HRESULT DirTreeNode::WriteStrKeyTree(CodeWriter& _CodeWriter, bool _bEraseExtension)
{
	if (false == IsRoot())
	{
		std::string strCode = "namespace ";
		strCode += m_DirName.filename().string();
		_CodeWriter.WriteCode(0, strCode);
	}

	//중괄호 열고 자신의 파일목록 작성
	{
		_CodeWriter.OpenBracket(0);

		size_t size = m_vecFileName.size();
		for (size_t i = 0; i < size; ++i)
		{
			std::string strCode = "STRKEY ";
			{
				std::string varName;
				
				if (_bEraseExtension)
					varName = m_vecFileName[i].filename().replace_extension("").string();
				else
					varName = m_vecFileName[i].filename().string();

				//변수명에 사용할 수 없는 특수문자를 제외
				strCode += std::regex_replace(varName, define_Preset::Regex::g_VarForbiddenChars::A, "_");

			}




			strCode += " = \"";

			if (false == IsRoot())
			{
				std::string tempstring = m_DirName.string();
				std::replace(tempstring.begin(), tempstring.end(), '\\', '/');
				strCode += tempstring;
				strCode += "/";
			}


			if (_bEraseExtension)
				strCode += m_vecFileName[i].replace_extension("").string();
			else
				strCode += m_vecFileName[i].string();


			strCode += "\";";
			_CodeWriter.WriteCode(0, strCode);
		}
	}

	//자식 노드가 있을 경우 재귀 호출 
	{
		size_t size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			HRESULT hr = m_vecChild[i]->WriteStrKeyTree(_CodeWriter, _bEraseExtension);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
		}
	}

	_CodeWriter.CloseBracket(0);
	_CodeWriter.WriteCode(0);

	return S_OK;
}
