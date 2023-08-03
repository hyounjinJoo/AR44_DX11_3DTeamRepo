#pragma once

void CreateScriptCPP();

//정규화된 이름 규칙을 가진 쉐이더들의 코드를 자동 생성
//여기서 자동 생성하지 않아도 직접 생성할 수 있음.
void CreateShaderCode();
void CreateGraphicsShaderCode(std::ofstream& _outFile_StrKey);
void CreateCShaderCode(std::ofstream& _outFile_StrKey);




