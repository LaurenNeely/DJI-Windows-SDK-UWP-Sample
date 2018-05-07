// dll_prefix.h - Control export or import for sdk headers
#pragma once  

#ifdef DJIWINDOWSSDK_EXPORTS  
#define DJISDK_API __declspec(dllexport)   
#else  
#define DJISDK_API __declspec(dllimport)   
#endif  