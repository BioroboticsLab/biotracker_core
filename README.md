# README

[![Build Status](https://git.imp.fu-berlin.de/bioroboticslab/biotracker/biotracker/badges/master/build.svg)](https://git.imp.fu-berlin.de/bioroboticslab/biotracker/biotracker/pipelines)

# README

## Using BioTracker

See the [wiki](https://github.com/BioroboticsLab/biotracker_core/wiki).

## Running BioTracker from prebuild binaries:

The releases github page provides stable version. 
Most recent binaries can be grabbed from the pipeline.  
- [BioTracker core application](https://git.imp.fu-berlin.de/bioroboticslab/biotracker/biotracker/pipelines)  
- Some trackers. For example the [Background subtraction tracker](https://git.imp.fu-berlin.de/bioroboticslab/biotracker/backgroundsubtraction_tracker/pipelines)

## Building trackers against prebuild BioTracker

This will enable you to only build your own plugin without having to care about the robotracker toolchaining/dependencies/etc..  
- Download [Interfaces](https://git.imp.fu-berlin.de/bioroboticslab/biotracker/interfaces/pipelines) project and add it to your toolchain (e.g. cmake prefix path) 
- Optional: Do the same for the [utility](https://git.imp.fu-berlin.de/bioroboticslab/biotracker/utility/pipelines) project, if you need it  
- Download and install the [BioTracker core application](https://git.imp.fu-berlin.de/bioroboticslab/biotracker/biotracker/pipelines)  
- It might be helpful to use some basic tracker as a template: https://github.com/BioroboticsLab/biotracker_sampletracker  
- Make sure to implement the IBehavior interface  
- Build  
- Put your artifact in the Plugins section  
- Run and Enjoy  

##  Build dependencies

Building the Robotracker needs: 
- opencv (> 3.0)  
- QT (>= 5.4)  
- Boost 
- CMake (>= 3.13)  
- Buildtools (Tested: MSVC buildtools or g++)  
- recommendet: ninja  

##  Building BioTracker (Windows x64)

The [dockerfile](https://git.imp.fu-berlin.de/bioroboticslab/robofish/docker) documents the dependency installation: (includes dependencies for all projects, e.g. BioTracker, interfaces, ...)  
  
Install and configure those dependencies. A shorthand to the dockerfile is using [vcpkg](https://git.imp.fu-berlin.de/bioroboticslab/robofish/vcpkg) 
Get the boost and opencv packages:  
` vcpkg install boost-property-tree:x64-windows-14.16 boost-bimap:x64-windows-14.16 boost-assign:x64-windows-14.16 boost-system:x64-windows-14.16 boost-filesystem:x64-windows-14.16 boost-chrono:x64-windows-14.16 boost-timer:x64-windows-14.16 boost-program-options:x64-windows-14.16 `  
`vcpkg install openblas:x64-windows-14.16 opencv[opengl,ffmpeg,ximea,cuda]:x64-windows-14.16 `  
... and Qt systemwide using Qt5_dir  
Now call cmake with some switches to include vcpkg:   `-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-14.16`  
... and build. You might need to copy flang DLL files, as their vcpkg integration is not yet complete.   
Proceed withe the build steps as in the linux section.

##  Building BioTracker (Linux)

The [dockerfile](https://git.imp.fu-berlin.de/bioroboticslab/robofish/docker) documents the dependency installation: (includes dependencies for all projects, e.g. BioTracker, interfaces, ...)   
Having set all the library paths you can build it just like any cmake project. It's split into a few repositories for modularization. Clone, build and install them in order:   [Interfaces](https://github.com/BioroboticsLab/biotracker_interfaces), [Utility](https://github.com/BioroboticsLab/biotracker_utility), [Behavior loader](https://github.com/BioroboticsLab/behavior_loader), the [BioTracker](https://github.com/BioroboticsLab/biotracker_core) itself and any tracking plugin of your liking, eg. the [Background subtraction tracker](https://github.com/BioroboticsLab/biotracker_backgroundsubtraction_tracker).

##  Building BioTracker (OSX)

Technically the BioTracker should build using the CMake toolchain and run on OSX. This is not officially supported, though.
