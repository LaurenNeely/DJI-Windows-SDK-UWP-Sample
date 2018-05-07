#include <plog/Log.h>
#include "modulemediator.h"
#include "../include/djivideoparser.h"

namespace dji
{
    namespace videoparser
    {
        
        ModuleMediator* g_pModuleMediator = new ModuleMediator;
        
        bool Initialize()
        {
            return g_pModuleMediator->Initialize();
        }
        
        void Uninitialize()
        {
            return g_pModuleMediator->Uninitialize();
        }
        
#ifdef _ANDROID_

        bool SetWindow(int product_id, int product_type, int index, void* window)
        {
            std::shared_ptr<dji::videoparser::VideoParserMgr> video_parser_mgr = g_pModuleMediator->GetVideoParserMgr().lock();
            
            if (video_parser_mgr)
            {
                return video_parser_mgr->SetWindow(product_id, product_type, index, window);
            }

            return false;
        }
        
#endif
        
#ifdef _PC_
        
        void set_callback(int product_id, int index, std::function<void(uint8_t *data, int width, int height)> func)
        {
            std::shared_ptr<dji::videoparser::VideoParserMgr> video_parser_mgr = g_pModuleMediator->GetVideoParserMgr().lock();
            
            if (video_parser_mgr)
            {
                video_parser_mgr->SetWindow(product_id, index, func);
            }
        }
        
        
#endif
        
        void set_parser_data(int product_id, int index, const unsigned char *data, size_t data_length)
        {
//            LOGD << "get video data " << data.size();
            std::shared_ptr<dji::videoparser::VideoParserMgr> video_parser_mgr = g_pModuleMediator->GetVideoParserMgr().lock();
            
            if(video_parser_mgr)
            {
//                LOGD << "video_parser_mgr->ParserData ";
                video_parser_mgr->ParserData(product_id, index, data, data_length);
            }
        }
        
       	
    }
}
