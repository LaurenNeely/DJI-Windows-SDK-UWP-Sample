using DJISDK;
using System;
using System.Runtime.InteropServices;
using Windows.Storage.Streams;

namespace DJIDemo
{
    public class JoyStickValues
    {
        public JoyStickValues(double? throttle, double? roll, double? pitch, double? yaw)
        {
            this.throttle = throttle.ConvertJoystickValue();
            this.roll = roll.ConvertJoystickValue();
            this.pitch = pitch.ConvertJoystickValue();
            this.yaw = yaw.ConvertJoystickValue();
        }

        public float? throttle;
        public float? roll;
        public float? pitch;
        public float? yaw;
    }

    public sealed class DJIClient
    {
        private static volatile DJIClient instance;
        private static object syncRoot = new Object();
        private double velocityX, velocityY, velocityZ;
        private double pitch, yaw, roll;
        private double altitude;

        private DJIClient() { }

        public delegate void DJIBoolEventHandler(bool newValue);
        public delegate void DJIDoubleEventHandler(double newValue);
        public delegate void DJIAttitudeEventHandler(double pitch, double yaw, double roll);
        public delegate void DJIVelocityEventHandler(double X, double Y, double Z);
        public event DJIBoolEventHandler FlyingChanged;
        public event DJIBoolEventHandler ConnectedChanged;
        public event DJIDoubleEventHandler AltitudeChanged;
        public event DJIAttitudeEventHandler AttitudeChanged;
        public event DJIVelocityEventHandler VelocityChanged;
        DJIClientNative.BoolCallback flyingCallback;
        DJIClientNative.BoolCallback connectedCallback;
        DJIClientNative.DoubleCallback altitudeCallback;
        DJIClientNative.AttitudeCallback attitudeCallback;
        DJIClientNative.VelocityCallback velocityHandler;
        DJIClientNative.VideoFrameBufferCallback videoFrameBufferCallback;

        public delegate void VideoEventHandler(IBuffer buffer, uint width, uint height, ulong timeStamp);
        public event VideoEventHandler FrameArived;

        public static DJIClient Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (syncRoot)
                    {
                        if (instance == null)
                        {
                            instance = new DJIClient();
                        }
                    }
                }

                return instance;
            }
        }

        /// <summary>
        /// Start the connection to the DJI Drone. It is advised to attach handlers to events prior to calling this method.
        /// </summary>
        public void Initialize()
        {
            flyingCallback = Instance.OnFlyingChanged;
            connectedCallback = Instance.OnConnected;
            altitudeCallback = Instance.OnAltitudeChanged;
            attitudeCallback = Instance.OnAttitudeChanged;
            velocityHandler = Instance.OnVelocityChanged;
            videoFrameBufferCallback = Instance.OnVideoData;

            DJIClientNative.InitializeDJISDK(
                videoFrameBufferCallback,
                connectedCallback, 
                flyingCallback, 
                altitudeCallback, 
                attitudeCallback,
                velocityHandler);
        }

        private void OnVelocityChanged(double X, double Y, double Z)
        {
            velocityX = X;
            velocityY = Y;
            velocityZ = Z;
            VelocityChanged?.Invoke(velocityX, velocityY, velocityZ);
        }

        private void OnAttitudeChanged(double pitch, double yaw, double roll)
        {
            this.pitch = pitch;
            this.yaw = yaw;
            this.roll = roll;
            AttitudeChanged?.Invoke(pitch, yaw, roll);
        }

        private void OnAltitudeChanged(double newValue)
        {
            altitude = newValue;
            AltitudeChanged?.Invoke(altitude);
        }

        public void UnInitialize()
        {
            DJIClientNative.UninitializeDJISDK();
        }

        public bool IsFlying { get; private set; } = false;

        public bool IsConnected { get; private set; } = false;

        public void SetJoyStickValue(float throttle, float roll, float pitch, float yaw)
        {
            DJIClientNative.SetJoyStickValue(throttle, roll, pitch, yaw);
        }

        public void SetGimbleAngle(double pitch, double yaw = 0, double roll = 0, bool pitchControlInvalid = false, bool rollControlInvalid = false, bool yawControlInvalid = false, double time = 1, double mode = 1)
        {
            DJIClientNative.SetGimbleAngle(pitch, yaw, roll, pitchControlInvalid, rollControlInvalid, yawControlInvalid, time, mode);
        }

        private void OnFlyingChanged(bool flying)
        {
            if (flying != IsFlying)
            {
                IsFlying = flying;
                FlyingChanged?.Invoke(flying);
            }
        }

        private void OnVideoData(IntPtr frameIBufferPtr, uint width, uint height, ulong timeStamp)
        {
            IBuffer buffer = Marshal.GetObjectForIUnknown(frameIBufferPtr) as IBuffer;
            FrameArived?.Invoke(buffer, width, height, timeStamp);
        }

        private void OnConnected(bool connected)
        {
            if (connected != IsConnected)
            {
                IsConnected = connected;
                ConnectedChanged?.Invoke(connected);
            }
        }
    }
}
