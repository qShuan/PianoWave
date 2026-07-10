@echo off

pushd ..
premake\premake5.exe --file=build.lua vs2022

echo Running action 'export-compile-commands'...
premake\premake5.exe --file=build.lua export-compile-commands >nul 2>&1
echo Done

if exist compile_commands\debug.json (
    echo Copying compile_commands\debug.json -^> compile_commands.json...
    copy /Y compile_commands\debug.json compile_commands.json >nul
    echo Done
)

echo Finished

popd
pause
