
:: Take Arguments From Build Event
set Configuration=%1

:: 만약 Argument가 들어오지 않았을 경우 기본값 설정
if "%Configuration%"=="" set Configuration=Debug

:: 메시지 표시
echo Running %0... 
echo Mode: %Configuration%

:: *.dll 파일 복사
xcopy /d /s /y /i /r .\External\DLL\%Configuration%\*.dll .\Output\%Configuration%


:: GameResource 파일 복사
if "%Configuration%"=="Release" ( 
	if not exist .\Output\Release\Res ( mkdir .\Output\Release\Res ) 
	xcopy /d /s /y /i /r .\Output\Debug\Res\* .\Output\Release\Res 
)



:: 1번 Argument에 아무것도 들어오지 않았을 경우(== 직접 실행했을 경우) 일시 정지
if "%1"=="" pause
