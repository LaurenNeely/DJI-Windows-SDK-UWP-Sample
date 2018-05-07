// gimbalhandler.h - Gimbal's handler.
#pragma once
#include <functional>
#include <stdint.h>
#include "valuesdef.h"
#include "../dllprefix.h"

namespace dji{
    namespace windowssdk{

        class DJISDK_API GimbalHandler final
        {
        public:
            GimbalHandler(const GimbalHandler&) = delete;
            GimbalHandler& operator= (const GimbalHandler &) = delete;

            uint8_t GetProductIndex();

            void UnregisterPushListener(uint64_t listen_id);

            uint8_t GetComponentIndex();

            /**
             * key : GimbalAttitudeInDegree
             * description : Gimbal attitude push
             */
            void GetGimbalAttitudeInDegree(std::function<void(int ret_code, const GimbalAttitudeMsg *value)> callback);
            uint64_t RegisterListenGimbalAttitudeInDegreePush(std::function<void(const GimbalAttitudeMsg* old_value, const GimbalAttitudeMsg* new_value)> callback);

            /**
             * key : RotateAngle
             * description : Rotate gimbal with angle value
             */
            void RotateAngle(const GimbalRotateAngleMsg *value, std::function<void(int ret_code)>);
            /**
             * key : RotateSpeed
             * description : Rotate gimbal with speed
             */
            void RotateSpeed(const GimbalRotateSpeedMsg *value, std::function<void(int ret_code)>);
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

            GimbalHandler();
            GimbalHandler(uint8_t product_id, uint8_t component_index);
        };

    }
}
