using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DJISDK
{
    public static class DJIClientNative
    {
#if NETFX_CORE
        public delegate void VideoFrameBufferCallback(IntPtr frameIBufferPtr, uint width, uint height, ulong timeStamp);
#else
        public delegate void VideoFrameDataCallback(IntPtr videoDataPtr, uint width, uint height, uint bpp, ulong timeStamp);
#endif

        private static bool _initialized = false;
        private static Mutex _sdkMutex = new Mutex();

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void BoolCallback(bool connected);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void DoubleCallback(double newValue);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void AttitudeCallback(double pitch, double yaw, double roll);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VelocityCallback(double X, double Y, double Z);

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "InitializeDJISDK")]
        private static extern void _InitializeDJISDK(
            BoolCallback connectedCallback,
            BoolCallback isFlyingCallback,
            DoubleCallback altitudeCallback,
            AttitudeCallback attitudeCallback,
            VelocityCallback velocityCallback
            );

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "UninitializeDJISDK")]
        private static extern void _UninitializeDJISDK();

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "IsReady")]
        private static extern int _IsReady();

#if NETFX_CORE
        public static void InitializeDJISDK(VideoFrameBufferCallback videoCallback,
#else
        public static void InitializeDJISDK(VideoFrameDataCallback videoCallback,
#endif
            BoolCallback connectedCallback,
            BoolCallback isFlyingCallback,
            DoubleCallback altitudeCallback,
            AttitudeCallback attitudeCallback,
            VelocityCallback velocityCallback
            )
        {
            _sdkMutex.WaitOne();
            try
            {
                _InitializeDJISDK(connectedCallback, isFlyingCallback, altitudeCallback, attitudeCallback, velocityCallback);
                _initialized = true;
#if NETFX_CORE
                _SetVideoFrameBufferCallback(videoCallback);
#else
                _SetVideoFrameDataCallback(videoCallback);
#endif
            }
            finally
            {
                _sdkMutex.ReleaseMutex();
            }
        }

        public static void UninitializeDJISDK()
        {
            _sdkMutex.WaitOne();
            try
            {
                _UninitializeVideoCallbacks();
                _UninitializeDJISDK();
                _initialized = false;
            }
            finally
            {
                _sdkMutex.ReleaseMutex();
            }
        }

        public static bool IsReady()
        {
            bool bReady = false;

            _sdkMutex.WaitOne();
            try
            {
                bReady = _initialized;
                //bReady &= (_IsReady() > 0);
            }
            finally
            {
                _sdkMutex.ReleaseMutex();
            }


            return bReady;
        }

#if NETFX_CORE
        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "SetVideoFrameBufferCallback")]
        private static extern void _SetVideoFrameBufferCallback(VideoFrameBufferCallback softwareBitmapCallback = null);
#else
        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "SetVideoFrameDataCallback")]
        private static extern void _SetVideoFrameDataCallback(VideoFrameDataCallback frameDataCallback = null);
#endif

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "UninitializeVideoCallbacks")]
        private static extern void _UninitializeVideoCallbacks();

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "IsFlying")]
        public static extern int IsFlying();

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "GetAltitude")]
        public static extern double GetAltitude();

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "GetAttitude")]
        public static extern void GetAttitude(ref double pitch, ref double yaw, ref double roll);

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "GetVelocity")]
        public static extern void GetVelocity(ref double velocityX, ref double velocityY, ref double velocityZ);

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "SetJoyStickValue")]
        public static extern void SetJoyStickValue(float throttle, float roll, float pitch, float yaw);

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "MoveGimbleValue")]
        public static extern void MoveGimbleValue(float pitch, float yaw = 0, float roll = 0);

        [DllImport("DJIClient", CallingConvention = CallingConvention.StdCall, EntryPoint = "SetGimbleAngle")]
        public static extern void SetGimbleAngle(double pitch, double yaw = 0, double roll = 0, bool pitchControlInvalid = false, bool rollControlInvalid = false, bool yawControlInvalid = false, double time = 1, double mode = 1);
    }
}
