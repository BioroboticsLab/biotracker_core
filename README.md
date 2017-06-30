# README

## Running Biotracker 3 from prebuild binaries:

Comming soon.

##  Building Biotracker 3 (Windows x64)

Building the Biotracker 3 needs: 
- opencv build (build yourself)
- QT build 
- Boost build
- CMake GUI (any version should be OK)

Following example will use Visual Studio 2015, OpenCv 3.2, QT 5.9 and boost_1_64, all x64.
You will need to tell CMake where to find the libraries. You can do this via GUI when asked or set enviromental variables.

Set:
- QT_DIR_CMAKE64, E.g. E:\Software\Libraries\QT\5.9\msvc2015_64\lib\cmake\Qt5
- CV_DIR_CMAKE64, E.g. E:\Software\Libraries\opencv\build
- Boost_LIB_DIR, E.g. E:\Software\Libraries\boost_1_64_0_VS14.0\lib64-msvc-14.0
- Boost_INCLUDE_DIRS, E.g. E:\Software\Libraries\boost_1_64_0_VS14.0
Note the versions in the path's and adjust if neccessary.

Getting boost to configure correctly is tricky, as it does not yet come with cmake files. Make sure the boost version strings match your used version. This is not automatically adjusted. The default is Boost_VERSION_STRING_DEB=-vc140-mt-gd-1_64, which is VS14 compiler, boost version 1_64.
Building with NVEnc is comming soon. Meanwhile ignore "HMNVLibDir=Not Found", it will build with CPU encoding only.

Now configure, generate, open and build for Debug/Release. The build is dynamically linked, which means you will need to supply the DLL's to your newly build binary. Qt offers the windeployqt utility. Usage sample, console:
C:\Users\Hauke>E:\Software\Libraries\QT\5.9\msvc2015_64\bin\windeployqt.exe E:\Development\Hauke\biotracker_core\BioTracker\CoreApp\BioTracker\Release\Biotracker_core.exe
OpenCv does not come with such a utility and you need to copy the DLL's manually from e.g.:
E:\Software\Libraries\opencv\build\bin\Release (or Debug, respectively)
Boost libraries are not needed to be copied.

Congratulations! You build the Biotracker 3!

##  Building Biotracker 3 (Linux)

Comming soon

##  Building Biotracker 3 (OSX)

Technically the Biotracker should build using the CMake toolchain and run on OSX. This is not officially supported, though.