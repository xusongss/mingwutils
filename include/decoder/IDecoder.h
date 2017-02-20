#ifndef _IDECODER_H_
#define _IDECODER_H_
#include<capture/Frame.h>
namespace Athena {
    class IDecoder {
    public:
        static const unsigned char CODE_FLAGS_QR =  0x01;
        static const unsigned char CODE_FLAGS_BAR = CODE_FLAGS_QR << 1;
        static const unsigned char CODE_FLAGS_DM =  CODE_FLAGS_QR << 2;
        static const unsigned char CODE_FLAGS_HX =  CODE_FLAGS_QR << 3;

        typedef enum Type{
            SYNC,
            ASYNC
        }Type_e;
        class Lisener{
        public:
            virtual void onDecode(unsigned char * out_buf, int out_len) = 0;
        };
        virtual int decoder(Frame_t * frame, unsigned char  * out_buf = 0, int * out_len = 0) = 0;

        virtual int setLisener(Lisener * l) = 0;
        virtual int setThreadNb(int threadNb) = 0;

    public:
        static IDecoder* createDecoder(Type_e type = ASYNC, unsigned char flags = CODE_FLAGS_QR);
        static void destroyDecoder(IDecoder*);
    };
}
#endif