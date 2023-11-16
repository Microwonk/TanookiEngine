@echo off

setlocal

:: Set the path to the glslc compiler (replace with the actual path)
set GLSLC_PATH="C:/VulkanSDK/1.3.261.1/Bin/glslc.exe"

:: Set the path to the shaders directory
set SHADERS_DIR=%CD%\shaders

:: Compile vertex and fragment shaders
for %%f in (%SHADERS_DIR%\*.vert %SHADERS_DIR%\*.frag) do (
    %GLSLC_PATH% "%%f" -o %SHADERS_DIR%\%%~nf.spv
)

endlocal

pause