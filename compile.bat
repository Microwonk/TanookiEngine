@echo off
setlocal

:: Set the path to the glslc compiler (replace with the actual path)
set GLSLC_PATH="C:/VulkanSDK/1.3.261.1/Bin/glslc.exe"

:: Set the path to the shaders directory
set SHADERS_DIR=%CD%\shaders

:: Remove existing .spv files in the directory
del /Q %SHADERS_DIR%\*.spv

:: Compile all shaders in the directory
for %%f in (%SHADERS_DIR%\*.*) do (
    %GLSLC_PATH% "%%f" -o %SHADERS_DIR%\%%~nxf.spv
)

endlocal
pause
