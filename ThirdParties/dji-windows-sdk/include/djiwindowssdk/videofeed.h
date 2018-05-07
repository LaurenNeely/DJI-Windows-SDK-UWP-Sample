// videofeed.h - Class represents one video feed from DJI products 
#pragma once

#include <functional>
#include <memory>
#include "dllprefix.h"

namespace dji {
	namespace windowssdk {
		class VideoFeedImpl; 

		class DJISDK_API VideoFeed
		{
		public:
			using VideoDataCallback = std::function<void(unsigned char *, size_t)>;
			~VideoFeed();

			int AddListener(VideoDataCallback listener_callback); 
			void RemoveListener(int listener_handler); 

		private:
			friend class VideoFeederImpl;
			VideoFeed(uint32_t product_id, uint8_t video_index);

			// members
			VideoFeedImpl *impl_ = nullptr; 
		};
	}
}