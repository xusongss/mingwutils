#ifndef _IDECODER_H_
#define _IDECODER_H_
#include<capture/Frame.h>
namespace Athena {
    class IDecoder {
    public:
        typedef enum Type{
            SYNC,
            ASYNC
        }Type_e;
        class Lisener{
            void onDecode(char * out_buf, int out_len);
        };
        virtual int decoder(Frame_t * frame, unsigned char  * out_buf, int * out_len) = 0;
        virtual int setLisener(Lisener * l) = 0;
        virtual int setThreadNb(int threadNb) = 0;

    public:
        static IDecoder* createDecoder(Type_e type);
        static void destroyDecoder(IDecoder*);
    };
}
#endif