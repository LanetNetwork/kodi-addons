@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
CLS
COLOR 0F
TITLE Kodi PVR Addon for LanetTV
SET comp=vs2013
SET target=dx
SET buildmode=ask
SET promptlevel=prompt

SET exitcode=0
FOR %%b in (%1, %2, %3, %4, %5) DO (
    IF %%b==vs2013 SET comp=vs2013
    IF %%b==dx SET target=dx
    IF %%b==gl SET target=gl
    IF %%b==clean SET buildmode=clean
)

SET buildconfig=Release

IF %comp%==vs2013 (
  REM look for MSBuild.exe in .NET Framework 4.x
  FOR /F "tokens=3* delims= " %%A IN ('REG QUERY HKLM\SOFTWARE\Microsoft\MSBuild\ToolsVersions\12.0 /v MSBuildToolsPath') DO SET NET=%%AMSBuild.exe
  IF NOT EXIST "!NET!" (
    FOR /F "tokens=3* delims= " %%A IN ('REG QUERY HKLM\SOFTWARE\Microsoft\MSBuild\ToolsVersions\12.0 /v MSBuildToolsPath') DO SET NET=%%AMSBuild.exe
  )

  IF EXIST "!NET!" (
    set msbuildemitsolution=1
    set OPTS_EXE=kodi-pvr-lanettv.sln /t:Build /p:Configuration="%buildconfig%"
    set CLEAN_EXE=kodi-pvr-lanettv.sln /t:Clean /p:Configuration="%buildconfig%"
  ) ELSE (
    IF "%VS120COMNTOOLS%"=="" (
        set NET="%ProgramFiles%\Microsoft Visual Studio 12.0\Common7\IDE\VCExpress.exe"
    ) ELSE IF EXIST "%VS120COMNTOOLS%\..\IDE\VCExpress.exe" (
        set NET="%VS120COMNTOOLS%\..\IDE\VCExpress.exe"
    ) ELSE IF EXIST "%VS120COMNTOOLS%\..\IDE\devenv.exe" (
        set NET="%VS120COMNTOOLS%\..\IDE\devenv.exe"
    )

    set OPTS_EXE=kodi-pvr-lanettv.sln /build %buildconfig%
    set CLEAN_EXE=kodi-pvr-lanettv.sln /clean %buildconfig%
  )
)

IF NOT EXIST %NET% (
   set DIETEXT=Visual Studio .NET 2012 was not found.
   goto DIE
)

ECHO Wait while preparing the build.
ECHO ------------------------------------------------------------
ECHO Cleaning Solution...
%NET% %CLEAN_EXE%
ECHO ------------------------------------------------------------
ECHO Compiling Addon for Kodi...
%NET% %OPTS_EXE%

IF EXIST ..\..\addons\pvr.lanettv-isengard\changelog.txt del ..\..\addons\pvr.lanettv-isengard\changelog.txt > NUL
IF EXIST ..\..\addons\pvr.lanettv-isengard\addon.xml del ..\..\addons\pvr.lanettv-isengard\addon.xml > NUL

SET BuildXML="%~dp0ConfigureAddonXML.bat"
CALL %BuildXML%
copy ..\..\changelog.txt ..\..\addons\pvr.lanettv-isengard\changelog.txt > NUL
SETLOCAL DISABLEDELAYEDEXPANSION


set ZIP="..\BuildDependencies\bin\7za.exe"
IF NOT EXIST %ZIP% (
   set DIETEXT=7zip was not found.
     goto DIE
)
for /F %%a in (%VERSION%) do SET "VERSION=%%a"

IF EXIST ..\..\pvr.lanettv-isengard.%VERSION%.zip del  ..\..\pvr.lanettv-isengard.%VERSION%.zip > NUL

%ZIP% a ..\..\pvr.lanettv-isengard.%VERSION%.zip ..\..\addons\pvr.lanettv-isengard -xr!*.in -xr!*.am -xr!*.exp -xr!*.ilk -xr!*.pdb -xr!*.lib -xr!.gitignore >NUL

goto END

:DIE
  ECHO ------------------------------------------------------------
  set DIETEXT=ERROR: %DIETEXT%
  echo %DIETEXT%
  SET exitcode=1
  ECHO ------------------------------------------------------------

:END
IF %promptlevel% NEQ noprompt (
ECHO ---------------------------------------------------------------
ECHO The file 'pvr.lanettv-isengard.%VERSION%.zip' was created in the root fork directory.
ECHO ---------------------------------------------------------------
ECHO Press any key to exit...
pause > NUL
)
EXIT /B %exitcode%
