#include "pch.h"
#include "DJIClientInterface.h"
#include <assert.h>
#include <dxgi.h>
#include <Windows.ui.xaml.media.dxinterop.h>
#include <Windows.ui.xaml.media.imaging.h>
#include <Robuffer.h>
#include <chrono>
#include "SearialTaskRunner.h"


using namespace ABI::Windows::UI::Xaml::Media::Imaging;

#pragma comment(lib, "windows_sdk.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "mfuuid.lib")

static bool g_sdkActivated = false;
static bool g_parserInitialized = false;
static bool g_deviceConnected = false;
static bool g_ready = false;
static bool g_isFlying = false;

static double g_altitude = -1;
static double g_pitch = -1;
static double g_yaw = -1;
static double g_roll = -1;
static double g_velocityX = -1;
static double g_velocityY = -1;
static double g_velocityZ = -1;

static std::recursive_mutex g_sdkMutex;
static std::mutex g_videoCallbackMutex;
static std::mutex g_videoAllocateMutex;
static dji::windowssdk::VideoFeed* g_videoFeed = nullptr;
static int g_videoFeedId = -1;

static unsigned int g_videoWidth = 0;
static unsigned int g_videoHeight = 0;

static Microsoft::WRL::ComPtr<ABI::Windows::Storage::Streams::IBufferFactory> g_bitmapBufferFactory;
static Microsoft::WRL::ComPtr<ABI::Windows::Storage::Streams::IBuffer> g_bitmapBuffer;
static Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> g_bitmapBufferAccess;

static VideoFrameBufferCallback g_frameBitmapCallback;
static VideoFrameDataCallback g_frameDataCallback;
static SerialTaskRunner g_frameTaskRunner;

static dji::windowssdk::VirtualRemoteController* g_virtualRemoteController = nullptr;
static VideoDataCallback g_videoDataCallback = nullptr;
static BoolCallback g_isFlyingCallback = nullptr;
static BoolCallback g_connectedCallback = nullptr;
static DoubleCallback g_altitudeCallback = nullptr;
static AttitudeCallback  g_attitudeCallback = nullptr;
static VelocityCallback  g_velocityCallback = nullptr;

extern "C" void DJI_CLIENT_API InitializeDJISDK(
    BoolCallback connectedCallback,
    BoolCallback isFlyingCallback,
    DoubleCallback altitudeCallback,
    AttitudeCallback attitudeCallback,
    VelocityCallback velocityCallback)
{
	std::lock_guard<std::recursive_mutex> lock(g_sdkMutex);

    g_connectedCallback = connectedCallback;
    g_isFlyingCallback = isFlyingCallback;
    g_altitudeCallback = altitudeCallback;
    g_attitudeCallback = attitudeCallback;
    g_velocityCallback = velocityCallback;

	if (!g_sdkActivated)
	{
		g_sdkActivated = true;
		dji::windowssdk::SDKManager::Instance().Activate();
		g_videoFeed = dji::windowssdk::SDKManager::Instance().GetVideoFeeder()->GetPrimaryVideoFeed();

		dji::windowssdk::SDKManager::Instance().GetComponentManager()->GetFlightControllerHandler(0, 0)->RegisterListenIsFlyingPush(
            [](const dji::windowssdk::CommonBoolValue* old_value, const dji::windowssdk::CommonBoolValue* new_value)
        {
            if (new_value && new_value->value != g_isFlying)
            {
                g_isFlying = new_value->value;
                if (g_isFlyingCallback != nullptr)
                    g_isFlyingCallback(g_isFlying);
            }
        });

		dji::windowssdk::SDKManager::Instance().GetComponentManager()->GetFlightControllerHandler(0, 0)->RegisterListenAltitudePush(
			[](const dji::windowssdk::CommonDoubleValue* old_value, const dji::windowssdk::CommonDoubleValue* new_value)
		{
            if (new_value && new_value->value != g_altitude)
            {
                g_altitude = new_value->value;
                if (g_altitudeCallback != nullptr)
                    g_altitudeCallback(g_altitude);
            }
        });
		
		dji::windowssdk::SDKManager::Instance().GetComponentManager()->GetFlightControllerHandler(0, 0)->RegisterListenAttitudePush(
			[](const dji::windowssdk::AttitudeValue* old_value, const dji::windowssdk::AttitudeValue* new_value)
		{
			if (new_value)
			{
				g_pitch = new_value->pitch;
				g_yaw = new_value->yaw;
				g_roll = new_value->roll;
                if (g_attitudeCallback != nullptr)
                    g_attitudeCallback(g_pitch, g_yaw, g_roll);
			}
		});

		dji::windowssdk::SDKManager::Instance().GetComponentManager()->GetFlightControllerHandler(0, 0)->RegisterListenVelocityPush(
			[](const dji::windowssdk::VelocityValue* old_value, const dji::windowssdk::VelocityValue* new_value)
		{
			if (new_value)
			{
				g_velocityX = new_value->speedX;
				g_velocityY = new_value->speedY;
				g_velocityZ = new_value->speedZ;
                if (g_velocityCallback != nullptr)
                    g_velocityCallback(g_velocityX, g_velocityY, g_velocityZ);
            }
		});

		dji::windowssdk::SDKManager::Instance().GetComponentManager()->GetFlightAssistantHandler(0, 0)->RegisterListenConnectionPush(
            [](const dji::windowssdk::CommonBoolValue* old_value, const dji::windowssdk::CommonBoolValue* new_value)
        {
            if (new_value && new_value->value != g_deviceConnected)
            {
                g_deviceConnected = new_value->value;
                if (g_connectedCallback != nullptr)
                    g_connectedCallback(g_deviceConnected);
            }
        });
	}

	if (g_sdkActivated && !g_parserInitialized)
	{
		g_parserInitialized = true;
		dji::videoparser::Initialize();

		dji::videoparser::set_callback(0, 0, ParserVideoCallback);
	}

	if (g_sdkActivated && g_parserInitialized && g_videoFeedId == -1)
	{
		g_videoFeedId = g_videoFeed->AddListener([](unsigned char * data, size_t size)
		{
			SDKFrameCallback(data, size);
		});
	}	
}


extern "C" void DJI_CLIENT_API SetVideoFrameBufferCallback(VideoFrameBufferCallback frameBitmapCallback)
{
	std::lock_guard<std::recursive_mutex> lock(g_sdkMutex);
	std::lock_guard<std::mutex> vlock(g_videoCallbackMutex);

	g_frameBitmapCallback = frameBitmapCallback;
}
extern "C" void DJI_CLIENT_API SetVideoFrameDataCallback(VideoFrameDataCallback frameDataCallback)
{
	std::lock_guard<std::recursive_mutex> lock(g_sdkMutex);
	std::lock_guard<std::mutex> vlock(g_videoCallbackMutex);
	g_frameDataCallback = frameDataCallback;
}
extern "C" void DJI_CLIENT_API UninitializeVideoCallbacks()
{
	std::lock_guard<std::recursive_mutex> lock(g_sdkMutex);
	std::lock_guard<std::mutex> vlock(g_videoCallbackMutex);
	std::lock_guard<std::mutex> alock(g_videoAllocateMutex);
	g_frameBitmapCallback = nullptr;
	g_frameDataCallback = nullptr;
}
extern "C" void DJI_CLIENT_API UninitializeDJISDK()
{
	std::lock_guard<std::recursive_mutex> lock(g_sdkMutex);
	std::lock_guard<std::mutex> vlock(g_videoCallbackMutex);
	std::lock_guard<std::mutex> alock(g_videoAllocateMutex);

	if (g_videoFeed != nullptr && g_videoFeedId != -1)
	{
		g_videoFeed->RemoveListener(g_videoFeedId);
		g_videoFeedId = -1;
	}

	if (g_parserInitialized)
	{
		dji::videoparser::Uninitialize();
		g_parserInitialized = false;
	}

    g_connectedCallback = nullptr;
    g_videoDataCallback = nullptr;
    g_isFlyingCallback = nullptr;
    g_altitudeCallback = nullptr;
    g_attitudeCallback = nullptr;
    g_velocityCallback = nullptr;
}


extern "C" int DJI_CLIENT_API IsDJISDKReady()
{
	bool bOK = g_deviceConnected;
	return bOK ? 1 : 0;
}

#pragma endregion FlightData

extern "C" int IsFlying()
{
	return g_isFlying ? 1 : 0;
}

extern "C" double GetAltitude()
{
	return g_altitude;
}

extern "C" void GetAttitude(double * pitch, double * yaw, double * roll)
{
	*pitch = g_pitch;
	*yaw = g_yaw;
	*roll = g_roll;
}

extern "C" void GetVelocity(double * speedX, double * speedY, double * speedZ)
{
	*speedX = g_velocityX;
	*speedY = g_velocityY;
	*speedZ = g_velocityZ;
}

#pragma endregion FlightData

#pragma region FlightControls

extern "C" void DJI_CLIENT_API SetJoyStickValue(float throttle, float roll, float pitch, float yaw)
{
	OutputDebugString(L"SetJoyStickValue");
	if (g_sdkActivated)
	{
		if (g_virtualRemoteController == nullptr)
		{
			g_virtualRemoteController = dji::windowssdk::SDKManager::Instance().GetVirtualRemoteController();
		}

		g_virtualRemoteController->SetJoyStickValue(throttle, roll, pitch, yaw);
	}
}

extern "C" void DJI_CLIENT_API MoveGimbleValue(float pitch, float yaw = 0, float roll = 0)
{
	dji::windowssdk::GimbalRotateSpeedMsg speed(pitch, yaw, roll);
	dji::windowssdk::SDKManager::Instance().GetComponentManager()->GetGimbalHandler(0, 0)->RotateSpeed(&speed, nullptr);
}


extern "C" void DJI_CLIENT_API SetGimbleAngle(double pitch, double yaw = 0, double roll = 0, bool pitchControlInvalid = false, bool rollControlInvalid = false, bool yawControlInvalid = false, double time = 1, double mode = 1)
{
	dji::windowssdk::GimbalRotationMode modeenum = dji::windowssdk::GimbalRotationMode::RELATIVE_ANGLE;
	if (mode)
	{
		modeenum = dji::windowssdk::GimbalRotationMode::ABSOLUTE_ANGLE;
	}

	dji::windowssdk::GimbalRotateAngleMsg speed(modeenum, pitch, roll, yaw, pitchControlInvalid, rollControlInvalid, yawControlInvalid, time);
	dji::windowssdk::SDKManager::Instance().GetComponentManager()->GetGimbalHandler(0, 0)->RotateAngle(&speed, nullptr);
}
#pragma endregion FlightControls


#pragma region Video


static void ParserVideoCallback(unsigned char *data, int width, int height)
{
	if (!data || !width || !height)
		return;

	HRESULT hr = S_OK;
	bool lockedByResize = false;

	unsigned __int64 now = (unsigned __int64)std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::lock_guard<std::mutex> vlock(g_videoCallbackMutex);

	if ((g_bitmapBufferAccess != nullptr || g_bitmapBuffer != nullptr)
		&& (width != g_videoWidth && height != g_videoHeight))
	{
		g_videoAllocateMutex.lock();
		lockedByResize = true;
		g_bitmapBuffer.Reset();
		g_bitmapBufferAccess.Reset();
	}

	if (g_bitmapBufferAccess == nullptr)
	{
		if (!lockedByResize)
		{
			g_videoAllocateMutex.lock();
		}

		g_bitmapBuffer.Reset();

		g_videoWidth = (unsigned int)width;
		g_videoHeight = (unsigned int)height;

		if (!g_bitmapBufferFactory)
		{
			ABI::Windows::Foundation::GetActivationFactory(
				Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_Storage_Streams_Buffer).Get(),
				&g_bitmapBufferFactory);
		}

		if (g_bitmapBufferFactory)
		{
			hr = g_bitmapBufferFactory->Create(g_videoWidth * g_videoHeight * DJI_FRAME_BPP, &g_bitmapBuffer);
			if (SUCCEEDED(hr))
			{
				g_bitmapBuffer->put_Length(g_videoWidth * g_videoHeight * DJI_FRAME_BPP);
				hr = g_bitmapBuffer.As(&g_bitmapBufferAccess);
			}
		}

		if (!lockedByResize)
		{
			g_videoAllocateMutex.unlock();
		}
	}

	if (g_bitmapBufferAccess)
	{
		byte* pTargetVideoData = nullptr;
		hr = g_bitmapBufferAccess->Buffer(&pTargetVideoData);

		if (pTargetVideoData)
		{
			assert(width == (int)g_videoWidth);
			assert(height == (int)g_videoHeight);

			memcpy_s(pTargetVideoData,
				g_videoWidth * g_videoHeight * DJI_FRAME_BPP,
				data,
				g_videoWidth * g_videoHeight * DJI_FRAME_BPP);
		}
	}

	g_frameTaskRunner.RunTask([now]()
	{
		if (!g_videoAllocateMutex.try_lock())
		{
			return;
		}

		try
		{
			if (g_frameBitmapCallback && g_bitmapBuffer)
			{
				g_frameBitmapCallback(g_bitmapBuffer.Get(), g_videoWidth, g_videoHeight, now);
			}

			if (g_frameDataCallback && g_bitmapBuffer)
			{
				byte* pTargetVideoData = nullptr;
				g_bitmapBufferAccess->Buffer(&pTargetVideoData);

				if (pTargetVideoData)
				{
					g_frameDataCallback(pTargetVideoData, g_videoWidth, g_videoHeight, DJI_FRAME_BPP, now);
				}
			}
		}
		catch (...)
		{
			OutputDebugStringW(L"!!! Unknow exception in the frame callback !!!");
		}

		g_videoAllocateMutex.unlock();
	});
}


static void SDKFrameCallback(unsigned char * data, size_t size)
{
	g_sdkMutex.lock();

	if (g_sdkActivated && g_parserInitialized)
	{
		g_deviceConnected = true; // even if haven't received "connected" signal, we got a frame, so we ARE connected 
		g_ready = true;

		dji::videoparser::set_parser_data(0, 0, data, size);
	}

	g_sdkMutex.unlock();
}
#pragma endregion Video


