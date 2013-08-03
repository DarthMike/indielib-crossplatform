SET DEVENVPATH="C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.com"
SET DEPENDENCIES_PATH=..\common\dependencies
%DEVENVPATH% %DEPENDENCIES_PATH%\FreeImage\FreeImage.2008.sln /build "Debug|Win32" /Project FreeImageLib
%DEVENVPATH% %DEPENDENCIES_PATH%\FreeImage\FreeImage.2008.sln /build "Debug|Win32" /Project FreeImageLib

%DEVENVPATH% %DEPENDENCIES_PATH%\glew-1.9.0\build\vc9\glew_static.sln /build "Debug|Win32"
%DEVENVPATH% %DEPENDENCIES_PATH%\glew-1.9.0\build\vc9\glew_static.sln /build "Release|Win32"

%DEVENVPATH% %DEPENDENCIES_PATH%\SDL-2.0\VisualC\SDL_VS2008.sln /build "Debug|Win32" /Project SDL2
%DEVENVPATH% %DEPENDENCIES_PATH%\SDL-2.0\VisualC\SDL_VS2008.sln /build "Release|Win32" /Project SDL2

%DEVENVPATH% %DEPENDENCIES_PATH%\SDL-2.0\VisualC\SDL_VS2008.sln /build "Debug|Win32" /Project SDL2main
%DEVENVPATH% %DEPENDENCIES_PATH%\SDL-2.0\VisualC\SDL_VS2008.sln /build "Release|Win32" /Project SDL2main