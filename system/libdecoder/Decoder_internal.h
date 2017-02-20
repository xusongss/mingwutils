#ifndef _DECODER_INTERNAL_H_
#define _DECODER_INTERNAL_H_
#include<decoder/IDecoder.h>
#include <utils/Thread.h>
#include <utils/Queue.h>
#include <utils/Mutex.h>
#include <stdio.h>
#include <list>
#include <map>

namespace Athena {

    class DecoderThread;
    /**
     *  class Decoder
     */
    class Decoder :public IDecoder{
    public:
        Decoder(Type_e type = ASYNC, unsigned char flags = CODE_FLAGS_QR, int threadNb = 3);
        ~ Decoder();
        virtual int decoder(Frame_t * frame, unsigned char  * buf, int * len);
        virtual int setLisener(Lisener * l);
    public:
        status_t start();
        status_t stop();

        int getThreadNb();
        int getRecv();
        int getDecodered();
        double getLossRate();
        status_t setThreadNb(int threadNb);
    public:
        char *getMem();
        void freeMem(char * pbuf);
    private:
        status_t init();
        status_t unInit();


    private:
        friend  class DecoderThread;
        /**
         *
         * @param msg
         * used by DecoderThread
         * the real decoder function
         */
        int  decoderFrame(Frame_t * frame,  unsigned char  * out_buf = NULL, int * out_len = NULL);
        /**
         * DecoderThread wile be pull Frame from ths Queue
         */
        sp<Queue> getMsgQue(){
            return mMsgQue;
        }
    private:
        sp<Queue>mMsgQue;

        bool mFirstFrame;
        bool mIsStarted;

        int mThreadNb;
        /**
         * mMemPoolLen = mThreadNb x 2
         * */
        int mMemPoolLen;
        char * mBasebuf;
        std::list< char* > mMemPool;
        std::map<int, sp<DecoderThread>>mThs;

        Mutex mMutex;
        Type_e mType;
        Lisener * mLisener;
        /**
         *Calculation Lost frame rate
         **/
        long long  mFrameRcvCount;
        long long  mFrameSendCount;

        /**
         * mPixFmt
         * mWidth
         * mHight
         *  the values is fixed until the time of first frame coming
         **/
        int mPixFmt;
        int mWidth;
        int mHight;

        const unsigned char mFlags;
    private:
        /**
         *
         * @param GetImage
         * @param iWidth
         * @param iHeight
         * @param pReslut
         * @param DataLength
         * @return
         *  1 decoder success
         *  0 decoder fails
         */
        static int QRdecode(unsigned char  *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength);
        static int HXdecode(unsigned char *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength);
        static int DMdecode(unsigned char *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength);
        static int BARdecode(unsigned char *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength);

    #define IMPLEMENT_DECODER_FUNCTION(class, name)\
        int class::name(unsigned char  *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength)\
        {\
             return Image##name(GetImage, iWidth, iHeight, pReslut, DataLength);\
        }
    };
}
#endif