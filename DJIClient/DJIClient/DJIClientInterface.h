#pragma once

#include <mutex>
#include <unordered_map>
#include <windows.media.core.h>
#include <windows.graphics.imaging.h>
#include <windows.graphics.imaging.interop.h>
#include <Mfapi.h>

#include "djisdk.h"

#define DJI_CLIENT_API DJISDK_API  __stdcall 
#define DJI_FRAME_BPP 4
#define DJI_FRAME_RATE 29.5

static void ParserVideoCallback(unsigned char *data, int width, int height);
static void SDKFrameCallback(unsigned char * data, size_t size);

extern "C" typedef void(__stdcall *VideoFrameBufferCallback)(ABI::Windows::Storage::Streams::IBuffer* frameBuffer, unsigned int uiWidth, unsigned int uiHeight, unsigned __int64 ulTimeStamp);
extern "C" typedef void(__stdcall *VideoFrameDataCallback)(void* videoDataBitmap, unsigned int uiWidth, unsigned int uiHeight, unsigned int bpp, unsigned __int64 ulTimeStamp);
extern "C" typedef void(__stdcall *DoubleCallback)(double newValue);
extern "C" typedef void(__stdcall *BoolCallback)(bool isTrue);
extern "C" typedef void(__stdcall *VideoDataCallback)(unsigned char* pRGBAData, unsigned int uiWidth, unsigned int uiHeight, unsigned __int64 ulTimeStamp);
extern "C" typedef void(__stdcall *AttitudeCallback)(double pitch, double yaw, double roll);
extern "C" typedef void(__stdcall *VelocityCallback)(double X, double Y, double Z);


