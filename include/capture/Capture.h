#ifndef _CAPTURE_H_
#define _CAPTURE_H_
#include<capture/Frame.h>
namespace Athena {
    class ICapture {
        class IFrameListener{
        public:
            virtual void onFrameAvailable(Frame * frame) = 0;
        };
        virtual status_t setFrameListener(IFrameListener * l) = 0;
        virtual status_t start() = 0;
        virtual status_t stop() = 0;
    };
}
#endif