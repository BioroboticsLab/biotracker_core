# README

## Running Biotracker 3 from prebuild binaries:

Binaries are distributed via Github: https://github.com/BioroboticsLab/biotracker_core/releases

##  Build dependencies

Building the Biotracker 3 needs: 
- opencv build (>3.0)
- QT build (>= 5.4)
- Boost build
- CMake GUI (any version should be OK, but boost detection may vary)

##  Building Biotracker 3 (Windows x64)

Following example will use Visual Studio 2015, OpenCv 3.2, QT 5.9 and boost_1_64, all x64.
You will need to tell CMake where to find the libraries. You can do this via GUI when asked or set enviromental variables.

Set:
- QT_DIR_CMAKE64, E.g. E:\Software\Libraries\QT\5.9\msvc2015_64\lib\cmake\Qt5
- CV_DIR_CMAKE64, E.g. E:\Software\Libraries\opencv\build
- BOOST_LIBRARYDIR, E.g. E:\Software\Libraries\boost_1_65_1\stage64\lib
- BOOST_ROOT, E.g. E:\Software\Libraries\boost_1_65_1
Note that your path's and library version may differ.

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