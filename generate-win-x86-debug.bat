@rem OSR mode is enabled by default, add -DUSE_OSR=OFF to disable the OSR mode
cmake -S . ^
-B .build/windows.x86.debug ^
-A Win32 ^
-DPROJECT_ARCH=x86 ^
-DBUILD_DEMO=ON ^
-DCMAKE_BUILD_TYPE=DEBUG ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.x86.debug" ^
-DZLIB_ROOT=c:/qt6/build/depends_x86