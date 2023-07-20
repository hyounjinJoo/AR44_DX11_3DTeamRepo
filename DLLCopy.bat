
:: Take Arguments From Build Event
set Platform=%1
set Configuration=%2

:: 만약 Argument가 들어오지 않았을 경우 기본값 설정
if "%Platform%"=="" set Platform=x64
if "%Configuration%"=="" set Configuration=Debug

:: 메시지 표시
echo Running %0... 
echo Mode: %Configuration%

:: *.dll 파일 복사
xcopy /d /s /y /i /r .\External\dll\*.dll .\%Platform%\%Configuration%



:: GameResource 파일 복사
if not exist .\%Platform%\%Configuration%\GameResources ( mkdir .\%Platform%\%Configuration%\GameResources )
xcopy /d /s /y /i /r .\GameResources\* .\%Platform%\%Configuration%\GameResources


:: 1번 Argument에 아무것도 들어오지 않았을 경우(== 직접 실행했을 경우) 일시 정지
if "%2"=="" pause
