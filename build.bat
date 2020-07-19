@echo off

pushd build
    call cl.exe /nologo /FeTOSave2K11.exe /Zi /I ..\imgui ..\src\main.cpp ..\imgui\*.cpp /link /subsystem:windows d3d9.lib Comdlg32.lib
popd