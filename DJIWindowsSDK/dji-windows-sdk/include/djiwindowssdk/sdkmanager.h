// sdkmanager.h - the root entry for DJI Windows SDK 
#pragma once  

#include "dllprefix.h"

#include "videofeeder.h"
#include "virtualremotecontroller.h"
#include "componentmanager.h"

namespace dji {
	namespace windowssdk {
		class DJISDK_API SDKManager
		{
		public:
			static SDKManager& Instance(); 
			~SDKManager();

			// Call this to activate DJI Windows SDK. 
			void Activate(); 

			VideoFeeder *GetVideoFeeder(); 
			VirtualRemoteController *GetVirtualRemoteController();
			ComponentManager *GetComponentManager();

		private:
			SDKManager();
		};
	}
}