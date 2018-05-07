// virtualremotecontroller.h - Class used to control the DJI drone with virtual joystick value.
#pragma once 
#include "dllprefix.h"
#include <functional>

namespace dji {
	namespace windowssdk {

		class DJISDK_API VirtualRemoteController final
		{
		public:
			void SetJoyStickValue(float throttle, float roll, float pitch, float yaw);

		private:
			friend class SDKManagerImpl;
			VirtualRemoteController();
			~VirtualRemoteController();

			class Impl;
			Impl *pImpl_;
		};
	}
}