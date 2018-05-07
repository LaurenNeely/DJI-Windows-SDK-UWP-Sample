#ifndef __video__
#define __video__
#include <memory>
#include <functional>

#ifdef WIN32
#ifdef VIDEO_PARSER_EXPORT
#define VIDEO_PARSER_API __declspec(dllexport)
#else
#define VIDEO_PARSER_API __declspec(dllimport)
#endif
#else
#define VIDEO_PARSER_API 
#endif

namespace dji
{
    namespace videoparser
    {
		VIDEO_PARSER_API bool Initialize();
		VIDEO_PARSER_API void Uninitialize();


		VIDEO_PARSER_API void set_callback(int product_id, int index, std::function<void(uint8_t *data, int width, int height)> func); 
		VIDEO_PARSER_API void set_parser_data(int product_id, int index, const unsigned char *data, size_t data_length);
    }
}
#endif
