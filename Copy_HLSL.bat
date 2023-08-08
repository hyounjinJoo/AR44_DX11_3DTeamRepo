
:: Take Arguments From Build Event
set Configuration=%1

:: 만약 Argument가 들어오지 않았을 경우 기본값 설정
if "%Configuration%"=="" set Configuration=Debug

:: 메시지 표시
echo Running %0... 
echo Mode: %Configuration%

set source_dir=.\Project\Engine\DefaultShader
set destination_dir=.\Project\HLSL
for /f %%f in (CopyList_HLSL_Header.txt) do (
    xcopy %source_dir%\%%f %destination_dir% /d /s /y /i /r

    :: 참조용 파일 읽기 전용으로 변경
    attrib +r %destination_dir%\%%f
)

:: 1번 Argument에 아무것도 들어오지 않았을 경우(== 직접 실행했을 경우) 일시 정지
if "%1"=="" pause
