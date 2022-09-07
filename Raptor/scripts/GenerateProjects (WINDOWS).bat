@echo off
pushd ..\
call vendor\tools\premake\premake5.exe vs2022
popd
pause