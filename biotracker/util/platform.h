#pragma once

/*
This macro must be used to mark all symbols that should be exported to a DLL.
*/
#ifdef _MSC_VER
#ifdef BUILD_BIOTRACKER_DLL
#define BIOTRACKER_DLLEXPORT __declspec(dllexport)
#else
#define BIOTRACKER_DLLEXPORT __declspec(dllimport)
#endif
#else
#define BIOTRACKER_DLLEXPORT
#endif
