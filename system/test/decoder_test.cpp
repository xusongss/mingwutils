#define LOG_TAG "decoder_test"
#include <decoder/IDecoder.h>
#include <capture/Frame.h>
#include <stdlib.h>
#include<unistd.h>
#include<utils/Log.h>
using namespace Athena;
int main(){
    IDecoder * decoder = IDecoder::createDecoder(IDecoder::ASYNC);
    for(int i = 0; i < 10000 ; i++){
        unsigned char * pbuf = (unsigned char * )malloc(128);
        Frame_t frame;
        frame.pBuff = pbuf;
        frame.size = 128;
        ALOGV("frame %d", i);
        decoder->decoder(&frame);
	free(pbuf);
       // usleep(1000);
    }
    IDecoder::destroyDecoder(decoder);
    //sleep(100000);

}
