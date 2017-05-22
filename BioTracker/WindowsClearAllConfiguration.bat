DEL ALL_BUILD.vcxproj
DEL ALL_BUILD.vcxproj.filters
DEL cmake_install.cmake
DEL ZERO_CHECK.vcxproj.filters
DEL ZERO_CHECK.vcxproj
DEL Biotracker.VC.db
DEL Biotracker.sln
DEL CMakeCache.txt
RMDIR /S /Q .vs
RMDIR /S /Q CMakeFiles
RMDIR /S /Q DEBUG
RMDIR /S /Q X64

CD CoreApp/BioTracker
DEL *.vcxproj.filters
DEL *.vcxproj.user
DEL *.vcxproj
DEL cmake_install.cmake
RMDIR /S /Q Biotracker_core.dir
RMDIR /S /Q Biotracker_core_autogen
RMDIR /S /Q CMakeFiles
RMDIR /S /Q .vs
CD ../..

CD Interfaces/BioTrackerInterfaces
DEL *.vcxproj.filters
DEL *.vcxproj.user
DEL *.vcxproj
DEL cmake_install.cmake
RMDIR /S /Q Biotracker_interfaces.dir
RMDIR /S /Q Biotracker_interfaces_autogen
RMDIR /S /Q CMakeFiles
RMDIR /S /Q .vs
CD ../..

CD Plugin/BioTrackerPlugin
DEL *.vcxproj.filters
DEL *.vcxproj
DEL *.vcxproj.user
DEL cmake_install.cmake
RMDIR /S /Q Biotracker_plugin_manual.dir
RMDIR /S /Q Biotracker_plugin.dir
RMDIR /S /Q Biotracker_plugin_autogen
RMDIR /S /Q Debug
RMDIR /S /Q Release
RMDIR /S /Q CMakeFiles
RMDIR /S /Q .vs
CD ../..


CD Utils/BioTrackerUtils
DEL *.vcxproj.filters
DEL *.vcxproj.user
DEL *.vcxproj
DEL cmake_install.cmake
RMDIR /S /Q Biotracker_utility.dir
RMDIR /S /Q Biotracker_utility_autogen
RMDIR /S /Q CMakeFiles
RMDIR /S /Q .vs
CD ../..

