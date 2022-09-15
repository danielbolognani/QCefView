@rem OSR mode is enabled by default, add -DUSE_OSR=OFF to disable the OSR mode
cmake -S . ^
-B .build/windows.x86_64.debug ^
-A x64 ^
-DPROJECT_ARCH=x86_64 ^
-DBUILD_DEMO=ON ^
-DCMAKE_BUILD_TYPE=DEBUG ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.x86_64.debug"