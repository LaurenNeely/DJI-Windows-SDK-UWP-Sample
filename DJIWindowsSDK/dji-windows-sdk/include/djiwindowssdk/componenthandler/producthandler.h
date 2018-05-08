// producthandler.h - Product's handler.
#pragma once
#include <functional>
#include <stdint.h>
#include "valuesdef.h"
#include "../dllprefix.h"

namespace dji{
    namespace windowssdk{

        class DJISDK_API ProductHandler final
        {
        public:
            ProductHandler(const ProductHandler&) = delete;
            ProductHandler& operator= (const ProductHandler &) = delete;

            uint8_t GetProductIndex();

            void UnregisterPushListener(uint64_t listen_id);

            
            /**
             * key : DeviceID
             * description : Product's device ID
             */
            void GetDeviceID(std::function<void(int ret_code, const CommonIntValue *value)> callback);
            uint64_t RegisterListenDeviceIDPush(std::function<void(const CommonIntValue* old_value, const CommonIntValue* new_value)> callback);

            /**
             * key : Connection
             * description : Is component connected.
             */
            void GetConnection(std::function<void(int ret_code, const CommonBoolValue *value)> callback);
            uint64_t RegisterListenConnectionPush(std::function<void(const CommonBoolValue* old_value, const CommonBoolValue* new_value)> callback);


            friend class ComponentManager;
        private:
            uint8_t product_index_;
            
            ProductHandler();
            ProductHandler(uint8_t product_id);
        };

    }
}
