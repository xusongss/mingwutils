#include<decoder/IDecoder.h>
#include <utils/Thread.h>
#include <utils/Queue.h>
#include <utils/Mutex.h>
#include <stdio.h>
#include <list>
#include <map>
namespace Athena{
    class Decoder;
    /**
     *  class DecoderThread
     */
    class DecoderThread :public Thread{
    public:
        DecoderThread(Decoder* decoder);
        virtual bool  threadLoop();

    private:
        Decoder * mDecoder;
        sp<Queue> mQue;
    };
    /**
     *  class Decoder
     */
    class Decoder :public IDecoder{
    public:
        Decoder(Type_e type = ASYNC, int threadNb = 3);
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
        void decoderFrame(msg_t * msg);
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
    };
    DecoderThread::DecoderThread(Decoder* decoder):
        mDecoder(decoder),mQue(decoder->getMsgQue()){
    }
    bool DecoderThread::threadLoop(){
        //sp<Queue> que = mDecoder->getMsgQue();
        msg_t *msg= mQue->dequeue(1000000000);
        if(msg != NULL){
            mDecoder->decoderFrame(msg);
        }
        return true;
    }

    Decoder::Decoder(Type_e type, int threadNb) :
            mType(type),
            mIsStarted(false),
            mFirstFrame(false),
            mThreadNb(threadNb),
            mFrameRcvCount(0),
            mFrameSendCount(0)
    {
        mMsgQue = new Queue;
    }
    int Decoder::setLisener(Lisener * Lis){
        AutoMutex l(mMutex);
        mLisener = Lis;
    }
    status_t Decoder::setThreadNb(int threadNb){
        bool current_state = mIsStarted;
        stop();
        mThreadNb = threadNb;
        if (current_state){
            return start();
        }
        return NO_ERROR;
    }
    int Decoder::getThreadNb(){
        return mThreadNb;
    }
    int Decoder::getRecv(){
        return mFrameRcvCount;
    }
    int Decoder::getDecodered(){
        return mFrameSendCount;
    }
    status_t Decoder::start(){
        AutoMutex l(mMutex);
        if (!mIsStarted){
            init();
        }
        std::map<int, sp<DecoderThread>>::iterator it = mThs.begin();
        for (; it != mThs.end(); ++it){
            sp<DecoderThread> pth = it->second;
            pth->run();
        }
        mIsStarted = true;
        return NO_ERROR;

    }
    status_t Decoder::stop(){
        AutoMutex l(mMutex);
        if (mIsStarted){
            mIsStarted = false;
            unInit();
        }
        return NO_ERROR;
    }
    char * Decoder::getMem(){
        AutoMutex l(mMutex);
        char* pBuf = NULL;
        if (mMemPool.size() > 0){
            pBuf = mMemPool.front();
            mMemPool.pop_front();
        }
        return pBuf;
    }
    void Decoder::freeMem(char * pbuf){
        AutoMutex l(mMutex);
        do{
            if (pbuf == NULL)
            {
                //LOGE(LOG_TAG, "%s buf null");
                break;
            }
            //memset(pbuf, 0, sizeof(pBuf));
            mMemPool.push_back(pbuf);

        } while (0);
    }
    double Decoder::getLossRate(){
        if (mFrameRcvCount > 0){
            return (mFrameRcvCount - mFrameSendCount) *100/ mFrameRcvCount;
        }
        return 0;
    }
    status_t Decoder::init(){
        mFirstFrame = true;
        int thNb = 0;
        for (thNb; thNb < mThreadNb; thNb++){
            sp<DecoderThread> pth = new DecoderThread(this);
            mThs[thNb] = pth;
        }
        mMemPoolLen = mThreadNb *2;
        return NO_ERROR;
    }
    status_t Decoder::unInit(){
        std::map<int, sp<DecoderThread>>::iterator it = mThs.begin();
        for (; it != mThs.end(); ++it){
            sp<DecoderThread> pth = it->second;
            pth->requestExitAndWait();
        }
        mThs.clear();

        std::list<char*>::iterator lit = mMemPool.begin();

        for (; lit != mMemPool.end(); ++lit){
            free(*lit);
        }
        mMemPool.clear();
        mFirstFrame = true;
        return NO_ERROR;

    }
    int Decoder::decoder(Frame_t * frame, unsigned char  * out_buf, int * out_len){
        mFrameRcvCount++;
        if (!mIsStarted){
            return 0;
        }
        if (mFirstFrame){
            int i = 0;
            AutoMutex l(mMutex);
            for (; i < mMemPoolLen; i++){
                char * pBuf = (char*)malloc(sizeof(msg_t)+frame->size + 32);
                if (pBuf == NULL){
                   // LOGE(LOG_TAG, "malloc null ");
                    return 0;
                }
                mMemPool.push_back(pBuf);
            }
            /*
            map<int, CDecodeThread*>::iterator it = mThs.begin();
            for (; it != mThs.end(); ++it){
                CDecodeThread* pth = it->second;
                pth->SetSubType(frame->GuidSubType);
                pth->SetWidth(frame->w);
                pth->SetHeight(frame->h);
            }
            */
            mFirstFrame = false;
        }
        char * pbuf = NULL;
        pbuf = this->getMem();
        if (pbuf == NULL)
        {
            //LOGE(LOG_TAG, "CBaseThread::GetMem() ");
            return 0;
        }
        msg_t* pMsg = (msg_t*)pbuf;
        pMsg->len = frame->size;
        memcpy(pMsg->data, frame->pBuff, frame->size);
        this->mMsgQue->enqueue(pMsg);
        mFrameSendCount++;
        return 0;
    }
    void Decoder::decoderFrame(msg_t * msg){
        this->freeMem((char*)msg);

    }
    IDecoder* IDecoder::createDecoder(Type_e type){
        return new Decoder(type);
    }
    void IDecoder::destroyDecoder(IDecoder* decoder){
        Decoder * self = (Decoder*)decoder;
        delete self;
    }

}
