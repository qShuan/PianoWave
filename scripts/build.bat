@echo off

pushd ..
premake\premake5.exe --file=build.lua vs2022
popd
pause