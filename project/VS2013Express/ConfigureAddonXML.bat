@ECHO OFF
SET CUR_PATH="%CD%"
SET SCRIPT_PATH="%~dp0"
SET SED="%~dp0..\BuildDependencies\bin\sed.exe"

cd "%SCRIPT_PATH%..\..\addons"

FOR /F "tokens=*" %%S IN ('dir /B "pvr.*"') DO (
    echo Configuring %%S xml
    cd "%%S"

    for /f "usebackq delims=" %%i in ("%CD%\..\src\lanettv\client.h") do (
            set "str=%%i"
            set "str1=!str:~8,18!"
            IF !str1! == PVR_CLIENT_VERSION (
                set VERSION=!str:~27!
            )

    )
    %SED% "s/@VERSION@/!VERSION!/" addon.xml.in > addon.xml
    cd ..\..
)

cd "%CUR_PATH%"

