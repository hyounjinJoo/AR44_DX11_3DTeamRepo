
:: Take Arguments From Build Event
set Platform=%1
set Configuration=%2

:: ���� Argument�� ������ �ʾ��� ��� �⺻�� ����
if "%Platform%"=="" set Platform=x64
if "%Configuration%"=="" set Configuration=Debug

:: �޽��� ǥ��
echo Running %0... 
echo Mode: %Configuration%

:: *.dll ���� ����
xcopy /d /s /y /i /r .\External\dll\*.dll .\%Platform%\%Configuration%

:: Shader ���� ����
if not exist .\%Platform%\%Configuration%\SHADER_SOURCE ( mkdir .\%Platform%\%Configuration%\SHADER_SOURCE )
xcopy /d /s /y /i /r .\SHADER_SOURCE\*.hlsl .\%Platform%\%Configuration%\SHADER_SOURCE
xcopy /d /s /y /i /r .\SHADER_SOURCE\*hlsli .\%Platform%\%Configuration%\SHADER_SOURCE

:: Resource ���� ����
if not exist .\%Platform%\%Configuration%\Resources ( mkdir .\%Platform%\%Configuration%\Resources )
xcopy /d /s /y /i /r .\Resources\* .\%Platform%\%Configuration%\Resources


:: 1�� Argument�� �ƹ��͵� ������ �ʾ��� ���(== ���� �������� ���) �Ͻ� ����
if "%2"=="" pause
