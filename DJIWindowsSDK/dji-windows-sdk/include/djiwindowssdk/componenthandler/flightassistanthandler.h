// flightassistanthandler.h - FlightAssistant's handler.
#pragma once
#include <functional>
#include <stdint.h>
#include "valuesdef.h"
#include "../dllprefix.h"

namespace dji{
    namespace windowssdk{

        class DJISDK_API FlightAssistantHandler final
        {
        public:
            FlightAssistantHandler(const FlightAssistantHandler&) = delete;
            FlightAssistantHandler& operator= (const FlightAssistantHandler &) = delete;

            uint8_t GetProductIndex();

            void UnregisterPushListener(uint64_t listen_id);

            uint8_t GetComponentIndex();

            /**
             * key : StartUAVDataPush
             * description : Start pushing UAV data push
             */
            void StartUAVDataPush(std::function<void(int ret_code)>);
            /**
             * key : StopUAVDataPush
             * description : Stop pushing UAV data 
             */
            void StopUAVDataPush(std::function<void(int ret_code)>);
            /**
             * key : UAVDataPushing
             * description : Is UAV data pushing. It would be set to false if no uav data pushing in 3 seconds.
             */
            void GetUAVDataPushing(std::function<void(int ret_code, const CommonBoolValue *value)> callback);
            uint64_t RegisterListenUAVDataPushingPush(std::function<void(const CommonBoolValue* old_value, const CommonBoolValue* new_value)> callback);

            /**
             * key : NavigationAircraftLocation
             * description : Aircraft location in navigation system
             */
            void GetNavigationAircraftLocation(std::function<void(int ret_code, const NavigationAircraftLocationValue *value)> callback);
            uint64_t RegisterListenNavigationAircraftLocationPush(std::function<void(const NavigationAircraftLocationValue* old_value, const NavigationAircraftLocationValue* new_value)> callback);

            /**
             * key : NavigationGimbalDirection
             * description : Gimbal direction in navigation system
             */
            void GetNavigationGimbalDirection(std::function<void(int ret_code, const NavigationGimbalDirectionValue *value)> callback);
            uint64_t RegisterListenNavigationGimbalDirectionPush(std::function<void(const NavigationGimbalDirectionValue* old_value, const NavigationGimbalDirectionValue* new_value)> callback);

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

            FlightAssistantHandler();
            FlightAssistantHandler(uint8_t product_id, uint8_t component_index);
        };

    }
}
