// valuesdef.h - the file that defines the values for handlers.
#pragma once
#include <string>
#include <vector>
#include "../dllprefix.h"

namespace dji
{
    namespace windowssdk
    {

        struct DJISDK_API DJIValue
        {
            virtual ~DJIValue() {};
            virtual bool operator == (const DJIValue& rhs) const = 0;
            virtual bool operator != (const DJIValue& rhs) const = 0;
        };

        enum class GimbalRotationMode{
			RELATIVE_ANGLE = 0, //The angle value, when the gimbal is rotating, relative to the current angle.
			ABSOLUTE_ANGLE = 1, //The angle value, when the gimbal is rotating, relative to 0 degrees (aircraft heading).
			UNKNOWN = 0xFFFF, //Unknown
		};
		
		enum class GPSSignalLevel{
			Level0 = 0, //	The GPS has almost no signal, which is very bad.
			Level1 = 1, //	The GPS signal is very weak.
			Level2 = 2, //	The GPS signal is weak. At this level, the aircraft's go home functionality will still work.
			Level3 = 3, //	The GPS signal is good. At this level, the aircraft can hover in the air.
			Level4 = 4, //	The GPS signal is very good. At this level, the aircraft can record the home point.
			Level5 = 5, //	The GPS signal is very strong.
			LevelNone = 6, //	There is no GPS signal.
			UNKNOWN = 0xFFFF, //	Unknown.
		};
		
		enum class USBMode{
			HOST = 1, //host mode
			SLAVE = 2, //slave mode
			UNKNOWN = 0xFFFF,
		};
		

        
		struct DJISDK_API CommonEmptyValue : public DJIValue
		{
		    CommonEmptyValue();
		    
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const CommonEmptyValue& rhs) const;
		
		
		};
		
		struct DJISDK_API CommonDoubleValue : public DJIValue
		{
		    CommonDoubleValue();
		    CommonDoubleValue(double value);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const CommonDoubleValue& rhs) const;
		
		    double value;
		};
		
		struct DJISDK_API CommonIntValue : public DJIValue
		{
		    CommonIntValue();
		    CommonIntValue(int value);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const CommonIntValue& rhs) const;
		
		    int value;
		};
		
		struct DJISDK_API CommonStringValue : public DJIValue
		{
		    CommonStringValue();
		    CommonStringValue(const std::string &value);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const CommonStringValue& rhs) const;
		
		    std::string value;
		};
		
		struct DJISDK_API CommonBoolValue : public DJIValue
		{
		    CommonBoolValue();
		    CommonBoolValue(bool value);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const CommonBoolValue& rhs) const;
		
		    bool value;
		};
		
		struct DJISDK_API GimbalAttitudeMsg : public DJIValue
		{
		    GimbalAttitudeMsg();
		    GimbalAttitudeMsg(double pitch, double roll, double yaw);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const GimbalAttitudeMsg& rhs) const;
		
		    double pitch;
		    double roll;
		    double yaw;
		};
		
		struct DJISDK_API GimbalRotateAngleMsg : public DJIValue
		{
		    GimbalRotateAngleMsg();
		    GimbalRotateAngleMsg(GimbalRotationMode mode, double pitch, double roll, double yaw, bool pitchControlInvalid, bool rollControlInvalid, bool yawControlInvalid, double time);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const GimbalRotateAngleMsg& rhs) const;
		
		    GimbalRotationMode mode; //Type of gimbal rotation operation mode.
		    double pitch; //The pitch property of a gimbal rotation operation, how much the gimbal rotates on the pitch axis.
		    double roll; //The roll property of a gimbal rotation operation, how much the gimbal rotates on the roll axis.
		    double yaw; //The yaw property of a gimbal rotation operation, how much the gimbal rotates on the yaw axis.
		    bool pitchControlInvalid; //If False, pitch angle may not change.
		    bool rollControlInvalid; //If False, roll angle may not change.
		    bool yawControlInvalid; //If False, yaw angle may not change.
		    double time; //The time of the gimbal speed rotation operation.
		};
		
		struct DJISDK_API GimbalRotateSpeedMsg : public DJIValue
		{
		    GimbalRotateSpeedMsg();
		    GimbalRotateSpeedMsg(double pitch, double yaw, double roll);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const GimbalRotateSpeedMsg& rhs) const;
		
		    double pitch;
		    double yaw;
		    double roll;
		};
		
		struct DJISDK_API CoordinateValue : public DJIValue
		{
		    CoordinateValue();
		    CoordinateValue(double latitude, double longitude);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const CoordinateValue& rhs) const;
		
		    double latitude;
		    double longitude;
		};
		
		struct DJISDK_API VelocityValue : public DJIValue
		{
		    VelocityValue();
		    VelocityValue(double speedX, double speedY, double speedZ);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const VelocityValue& rhs) const;
		
		    double speedX; //Current speed of the aircraft in the x direction, in meters per second, using the N-E-D (North-East-Down) coordinate system.
		    double speedY; //Current speed of the aircraft in the y direction, in meters per second, using the N-E-D (North-East-Down) coordinate system.
		    double speedZ; //Current speed of the aircraft in the z direction, in meters per second, using the N-E-D (North-East-Down) coordinate system.
		};
		
		struct DJISDK_API AttitudeValue : public DJIValue
		{
		    AttitudeValue();
		    AttitudeValue(double pitch, double roll, double yaw);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const AttitudeValue& rhs) const;
		
		    double pitch; //Aircraft's pitch attitude value.
		    double roll; //Aircraft's roll attitude value.
		    double yaw; //Aircraft's yaw attitude value.
		};
		
		struct DJISDK_API GPSSignalLevelValue : public DJIValue
		{
		    GPSSignalLevelValue();
		    GPSSignalLevelValue(GPSSignalLevel level);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const GPSSignalLevelValue& rhs) const;
		
		    GPSSignalLevel level;
		};
		
		struct DJISDK_API USBModeValue : public DJIValue
		{
		    USBModeValue();
		    USBModeValue(USBMode mode);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const USBModeValue& rhs) const;
		
		    USBMode mode; //usb mode of rc
		};
		
		struct DJISDK_API NavigationAircraftLocationValue : public DJIValue
		{
		    NavigationAircraftLocationValue();
		    NavigationAircraftLocationValue(double x, double y, double z);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const NavigationAircraftLocationValue& rhs) const;
		
		    double x;
		    double y;
		    double z;
		};
		
		struct DJISDK_API NavigationGimbalDirectionValue : public DJIValue
		{
		    NavigationGimbalDirectionValue();
		    NavigationGimbalDirectionValue(double pitch, double yaw, double roll);
		
		    virtual bool operator == (const DJIValue& rhs) const override;
		    virtual bool operator != (const DJIValue& rhs) const override;
		    virtual bool operator < (const NavigationGimbalDirectionValue& rhs) const;
		
		    double pitch;
		    double yaw;
		    double roll;
		};

    }
}
