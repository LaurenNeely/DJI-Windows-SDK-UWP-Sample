// remotecontrollerhandler.h - RemoteController's handler.
#pragma once
#include <functional>
#include <stdint.h>
#include "valuesdef.h"
#include "../dllprefix.h"

namespace dji{
    namespace windowssdk{

        class DJISDK_API RemoteControllerHandler final
        {
        public:
            RemoteControllerHandler(const RemoteControllerHandler&) = delete;
            RemoteControllerHandler& operator= (const RemoteControllerHandler &) = delete;

            uint8_t GetProductIndex();

            void UnregisterPushListener(uint64_t listen_id);

            uint8_t GetComponentIndex();

            /**
             * key : USBMode
             * description : Set/get RC USB mode.
             */
            void SetUSBMode(const USBModeValue *value, std::function<void(int ret_code)> callback);


            friend class ComponentManager;
        private:
            uint8_t product_index_;
            uint8_t component_index_;

            RemoteControllerHandler();
            RemoteControllerHandler(uint8_t product_id, uint8_t component_index);
        };

    }
}
