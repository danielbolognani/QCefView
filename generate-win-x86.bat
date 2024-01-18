@echo off
set OS_VERSION="win32"
set COMPILER_VERSION="vs2019"
set BUILD_TYPE="release"

::set ICU_VERSION="71.1"
set OPENSSL_VERSION="1.1.1t"
set ZLIB_VERSION="1.2.12"

if "%ADVTEC9_DIR%" neq "" (
	set CMAKE_PREFIX_PATH="%ADVTEC9_DIR%\third_libraries\openssl\%OPENSSL_VERSION%\%OS_VERSION%\include;%ADVTEC9_DIR%\third_libraries\openssl\%OPENSSL_VERSION%\%OS_VERSION%\%COMPILER_VERSION%\%BUILD_TYPE%;%ADVTEC9_DIR%\third_libraries\zlib\%ZLIB_VERSION%\%OS_VERSION%\include;%ADVTEC9_DIR%\third_libraries\zlib\%ZLIB_VERSION%\%OS_VERSION%\%COMPILER_VERSION%\%BUILD_TYPE%"
) else (
	if exist C:\advtec9\trunk\third_libraries (
		set ADVTEC9_DIR="C:\advtec9\trunk"
		set CMAKE_PREFIX_PATH="%ADVTEC9_DIR%\third_libraries\openssl\%OPENSSL_VERSION%\%OS_VERSION%\include;%ADVTEC9_DIR%\third_libraries\openssl\%OPENSSL_VERSION%\%OS_VERSION%\%COMPILER_VERSION%\%BUILD_TYPE%;%ADVTEC9_DIR%\third_libraries\zlib\%ZLIB_VERSION%\%OS_VERSION%\include;%ADVTEC9_DIR%\third_libraries\zlib\%ZLIB_VERSION%\%OS_VERSION%\%COMPILER_VERSION%\%BUILD_TYPE%"
	) else (
		echo "Variable ADVTEC9_DIR empty and directory C:\advtec9\trunk not found, execution halted."
		exit /b 1
	)
)

@echo on
@rem OSR mode is enabled by default, add -DUSE_OSR=OFF to disable the OSR mode
cmake -S . ^
-B .build/windows.x86.release ^
-A Win32 ^
-DPROJECT_ARCH=x86 ^
-DBUILD_DEMO=ON ^
-DCMAKE_BUILD_TYPE=RELEASE ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.x86.release" ^
-DZLIB_ROOT=c:/qt6/build/depends_x86