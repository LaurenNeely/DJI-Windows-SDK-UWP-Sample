// flightcontrollerhandler.h - FlightController's handler.
#pragma once
#include <functional>
#include <stdint.h>
#include "valuesdef.h"
#include "../dllprefix.h"

namespace dji{
    namespace windowssdk{

        class DJISDK_API FlightControllerHandler final
        {
        public:
            FlightControllerHandler(const FlightControllerHandler&) = delete;
            FlightControllerHandler& operator= (const FlightControllerHandler &) = delete;

            uint8_t GetProductIndex();

            void UnregisterPushListener(uint64_t listen_id);

            uint8_t GetComponentIndex();

            /**
             * key : StartTakeoff
             * description : Start aircraft takeoff.
             */
            void StartTakeoff(std::function<void(int ret_code)>);
            /**
             * key : StopTakeoff
             * description : Stop aircraft takeoff.
             */
            void StopTakeoff(std::function<void(int ret_code)>);
            /**
             * key : StartAutoLanding
             * description : Start aircraft auto-landing.
             */
            void StartAutoLanding(std::function<void(int ret_code)>);
            /**
             * key : StopAutoLanding
             * description : Stop aircraft auto-landing.
             */
            void StopAutoLanding(std::function<void(int ret_code)>);
            /**
             * key : AircraftLocation
             * description : The current location of the aircraft as a coordinate.
             */
            void GetAircraftLocation(std::function<void(int ret_code, const CoordinateValue *value)> callback);
            uint64_t RegisterListenAircraftLocationPush(std::function<void(const CoordinateValue* old_value, const CoordinateValue* new_value)> callback);

            /**
             * key : Velocity
             * description : Velocities of aircraft in three directions, in m/s, using the N-E-D (North-East-Down) coordinate system. 
             */
            void GetVelocity(std::function<void(int ret_code, const VelocityValue *value)> callback);
            uint64_t RegisterListenVelocityPush(std::function<void(const VelocityValue* old_value, const VelocityValue* new_value)> callback);

            /**
             * key : SatelliteCount
             * description : GPS satellite count.
             */
            void GetSatelliteCount(std::function<void(int ret_code, const CommonIntValue *value)> callback);
            uint64_t RegisterListenSatelliteCountPush(std::function<void(const CommonIntValue* old_value, const CommonIntValue* new_value)> callback);

            /**
             * key : Altitude
             * description : Relative altitude of the aircraft relative to take off location, measured by the barometer, in meters.
             */
            void GetAltitude(std::function<void(int ret_code, const CommonDoubleValue *value)> callback);
            uint64_t RegisterListenAltitudePush(std::function<void(const CommonDoubleValue* old_value, const CommonDoubleValue* new_value)> callback);

            /**
             * key : Attitude
             * description : The attitude of the aircraft, where the pitch, roll, and yaw values will be in the range of [-180, 180] degrees. If its pitch, roll, and yaw values are 0, the aircraft will be hovering level with a True North heading.
             */
            void GetAttitude(std::function<void(int ret_code, const AttitudeValue *value)> callback);
            uint64_t RegisterListenAttitudePush(std::function<void(const AttitudeValue* old_value, const AttitudeValue* new_value)> callback);

            /**
             * key : AreMotorsOn
             * description : If motors are on.
             */
            void GetAreMotorsOn(std::function<void(int ret_code, const CommonBoolValue *value)> callback);
            uint64_t RegisterListenAreMotorsOnPush(std::function<void(const CommonBoolValue* old_value, const CommonBoolValue* new_value)> callback);

            /**
             * key : IsFlying
             * description : If aircraft is flying.
             */
            void GetIsFlying(std::function<void(int ret_code, const CommonBoolValue *value)> callback);
            uint64_t RegisterListenIsFlyingPush(std::function<void(const CommonBoolValue* old_value, const CommonBoolValue* new_value)> callback);

            /**
             * key : FlightTimeInSeconds
             * description : The accumulated flight time, in seconds, since the aircraft was powered on.
             */
            void GetFlightTimeInSeconds(std::function<void(int ret_code, const CommonIntValue *value)> callback);
            uint64_t RegisterListenFlightTimeInSecondsPush(std::function<void(const CommonIntValue* old_value, const CommonIntValue* new_value)> callback);

            /**
             * key : GPSSignalLevel
             * description : GPS signal levels, which are used to measure the signal quality.
             */
            void GetGPSSignalLevel(std::function<void(int ret_code, const GPSSignalLevelValue *value)> callback);
            uint64_t RegisterListenGPSSignalLevelPush(std::function<void(const GPSSignalLevelValue* old_value, const GPSSignalLevelValue* new_value)> callback);

            /**
             * key : TakeOffFailedReason
             * description : Take off fail reason.
             */
            void GetTakeOffFailedReason(std::function<void(int ret_code, const CommonIntValue *value)> callback);
            uint64_t RegisterListenTakeOffFailedReasonPush(std::function<void(const CommonIntValue* old_value, const CommonIntValue* new_value)> callback);

            /**
             * key : Connection
             * description : Is component connected.
             */
            void GetConnection(std::function<void(int ret_code, const CommonBoolValue *value)> callback);
            uint64_t RegisterListenConnectionPush(std::function<void(const CommonBoolValue* old_value, const CommonBoolValue* new_value)> callback);


            friend class ComponentManager;
        private:
            uint8_t product_index_;
            uint8_t component_index_;

            FlightControllerHandler();
            FlightControllerHandler(uint8_t product_id, uint8_t component_index);
        };

    }
}
