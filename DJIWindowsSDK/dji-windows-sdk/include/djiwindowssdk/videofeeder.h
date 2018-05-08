// videofeeder.h - Class used to access video feeds from DJI products 
#pragma once

#include "dllprefix.h"

namespace dji {
	namespace windowssdk {
		class VideoFeed;
		class VideoFeederImpl; 

		class DJISDK_API VideoFeeder
		{
		public:
			~VideoFeeder();

			VideoFeed *GetPrimaryVideoFeed();
			

		private:
			friend class SDKManagerImpl; 
			VideoFeeder();

			// members
			VideoFeederImpl *impl_ = nullptr; 

		};
	}
}