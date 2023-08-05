#include "PCH_CodeGenerator.h"

#include "CodeWriter.h"

CodeWriter::CodeWriter()
	: m_vecBuffer(1)
{
}

CodeWriter::~CodeWriter()
{
}

void CodeWriter::WriteCode(UINT _BufferIdx, const std::string_view _strCode)
{
	MakeAvailable(_BufferIdx);

	//내려쓰기
	m_vecBuffer[_BufferIdx].Buffer += "\n";
	
	//공란으로 넣을 경우 한줄 내리기만 하고 끝
	if (true == _strCode.empty())
		return;

	//현재 들여쓰기 단계만큼 들여쓰기를 해준다.
	for (int i = 0; i < m_vecBuffer[_BufferIdx].iIndentation; ++i)
		m_vecBuffer[_BufferIdx].Buffer += "\t";

	//코드 내용 추가
	m_vecBuffer[_BufferIdx].Buffer += _strCode;
}


