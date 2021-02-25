@ECHO OFF
SET PRJDIR=%~dp0

IF NOT EXIST libamtrs\ (
	@ECHO Download libamtrs
	git clone git@github.com:isaponsoft/libamtrs.git libamtrs
)

cmake.exe %PRJDIR% -DCMAKE_BUILD_TYPE=Release && msbuild.exe /p:Configuration=Release addlicense.sln
