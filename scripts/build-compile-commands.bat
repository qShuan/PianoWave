@echo off

pushd ..
premake\premake5.exe --file=build.lua vs2022
premake\premake5.exe --file=build.lua export-compile-commands
copy /Y compile_commands\debug.json compile_commands.json
popd
pause
