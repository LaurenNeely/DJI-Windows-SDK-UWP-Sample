// componentmanager.h - Class used to deliver handlers for drone's components 
#pragma once
#include <stdint.h>
#include "dllprefix.h"

#include "componenthandler/flightAssistanthandler.h"
#include "componenthandler/producthandler.h"
#include "componenthandler/gimbalhandler.h"
#include "componenthandler/flightControllerhandler.h"
#include "componenthandler/remoteControllerhandler.h"

namespace dji
{
    namespace windowssdk
    {
        class DJISDK_API ComponentManager final
        {
        public:            
            ComponentManager(const ComponentManager&) = delete;
            ComponentManager& operator= (const ComponentManager &) = delete;

            FlightAssistantHandler* GetFlightAssistantHandler(uint8_t product_index, uint8_t component_index);
            ProductHandler* GetProductHandler(uint8_t product_index);
            GimbalHandler* GetGimbalHandler(uint8_t product_index, uint8_t component_index);
            FlightControllerHandler* GetFlightControllerHandler(uint8_t product_index, uint8_t component_index);
            RemoteControllerHandler* GetRemoteControllerHandler(uint8_t product_index, uint8_t component_index);
            
        private:
            friend class SDKManagerImpl;
            ComponentManager();
            ~ComponentManager();

            class impl;
            impl *pImpl = nullptr;
        };
    }
}
