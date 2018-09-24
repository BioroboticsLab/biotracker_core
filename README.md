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

Note: Building with NVEnc is comming soon. Meanwhile ignore "HMNVLibDir=Not Found", it will build with CPU encoding only.  
  
Install and configure the dependencies. E.g. use vcpkg: https://github.com/BioroboticsLab/RF_vcpkg  
Get the boost packages:  
vcpkg install  boost-property-tree:x64-windows-14.13 boost-bimap:x64-windows-14.13 boost-assign:x64-windows-14.13 boost-system:x64-windows-14.13 boost-filesystem:x64-windows-14.13 boost-chrono:x64-windows-14.13 boost-timer:x64-windows-14.13 boost-program-options:x64-windows-14.13  
vcpkg install openblas:x64-windows-14.13 opencv[opengl,ffmpeg,ximea,cuda]:x64-windows-14.13  
... and Qt systemwide using Qt5_dir  
Now call cmake with some switches to include vcpkg: -DCMAKE_TOOLCHAIN_FILE=C:/RF_vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-14.13  
... and build. You might need to copy flang DLL files, as their vcpkg integration is not yet complete.  
  
Congratulations! You build the Biotracker 3!  

##  Building Biotracker 3 (Linux)

Having set all the library paths you can build it just like any cmake project:  
git clone https://github.com/BioroboticsLab/biotracker_core.git  
cd biotracker_core  
mkdir build  
cd build  
cmake ../BioTracker  
make  

##  Building Biotracker 3 (OSX)

Technically the Biotracker should build using the CMake toolchain and run on OSX. This is not officially supported, though.