cmake -S . ^
-B .build/windows.x86.release ^
-A Win32 ^
-DPROJECT_ARCH=x86 ^
-DBUILD_DEMO=ON ^
-DCMAKE_BUILD_TYPE=RELEASE ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.x86.release"