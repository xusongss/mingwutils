#define LOG_TAG "Decoder"
//#define NDEBUG 0
#include <decoder/ImageDecoderFunc.h>
#include "Decoder_internal.h"
#include <utils/Log.h>
#include <capture/Frame.h>


namespace Athena{
    /**
 *  class DecoderThread
 */
    class DecoderThread :public Thread{
    public:
        DecoderThread(Decoder* decoder);
        ~DecoderThread();
        virtual status_t readyToRun();
        virtual bool  threadLoop();

    private:
        Decoder * mDecoder;
        sp<Queue> mQue;
    };

    DecoderThread::DecoderThread(Decoder* decoder):
        mDecoder(decoder){
        mQue = mDecoder->getMsgQue();
    }
    DecoderThread::~DecoderThread(){
        ALOGV("~DecoderThread is clall.");
    }
    status_t DecoderThread::readyToRun(){
        ALOGV("DecoderThread::readyToRun.");
        return NO_ERROR;
    }
    bool DecoderThread::threadLoop(){
        msg_t *msg= mQue->dequeue();
        if(msg != NULL){
            Frame_t * frame = (Frame_t*)msg->data;
            mDecoder->decoderFrame(frame);
            mDecoder->freeMem((char*)msg);
        }else{
            return false;
        }
        return true;
    }

    Decoder::Decoder(Type_e type, unsigned char flags,int threadNb) :
            mFlags(flags),
            mType(type),
            mBasebuf(NULL),
            mIsStarted(false),
            mFirstFrame(true),
            mThreadNb(threadNb),
            mFrameRcvCount(0),
            mFrameSendCount(0)
    {
        mMsgQue = new Queue;
    }
   Decoder:: ~ Decoder(){
        stop();
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
                ALOGE("free mem pbuf is null");
                break;
            }
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
        mMemPoolLen = mThreadNb * 2;
        return NO_ERROR;
    }
    status_t Decoder::unInit(){
        std::map<int, sp<DecoderThread>>::iterator it = mThs.begin();
        for (; it != mThs.end(); ++it){
            sp<DecoderThread> pth = it->second;
            pth->requestExit();
        }
        mMsgQue->notifyAll();
        mThs.clear();

        std::list<char*>::iterator lit = mMemPool.begin();

        if(mBasebuf){
            free(mBasebuf);
        }
        mMemPool.clear();
        mFirstFrame = true;
        return NO_ERROR;

    }
    int Decoder::decoder(Frame_t * frame, unsigned char  * out_buf, int * out_len){
        mFrameRcvCount++;
        if(this->mType == SYNC){
            return this->decoderFrame(frame, out_buf, out_len);
        }
        if (mFirstFrame){
            int i = 0;
            char * pBuf = NULL;
            int nodeLenth = sizeof(msg_t)+ frame->size + 32;

            this->start();

            AutoMutex l(mMutex);
            if(mMemPoolLen){
                mBasebuf = (char*)malloc(nodeLenth * mMemPoolLen);
            }
            pBuf = mBasebuf;
            for (; i < mMemPoolLen; i++){
                if (pBuf == NULL){
                    ALOGE(LOG_TAG, "init mem Pool pBuf null ");
                    return 0;
                }
                mMemPool.push_back(pBuf);
                pBuf += nodeLenth;
            }
            mFirstFrame = false;
        }
        if (!mIsStarted){
            return 0;
        }

        char * pbuf = NULL;
        pbuf = this->getMem();
        if (pbuf == NULL)
        {
            ALOGV("GetMem() is null ");
            return 0;
        }
        msg_t* pMsg = (msg_t*)pbuf;
        pMsg->len = frame->size;
        memcpy(pMsg->data, frame->pBuff, frame->size);
        this->mMsgQue->enqueue(pMsg);
        mFrameSendCount++;
        return NO_ERROR;
    }
    int Decoder::decoderFrame(Frame_t *frame,  unsigned char  * pout_buf, int * pout_len){

        ALOGE("Decoder::decoderFrame %d ", this->mFrameSendCount);
        Frame_t * pframe =frame;
        unsigned  char * pReslut = NULL;
        int len = 0;
        int ret = 0;
        unsigned char code_flag;
        do{
            if(mFlags & CODE_FLAGS_QR){
                ret = this->QRdecode(pframe->pBuff, pframe->width,pframe->height, &pReslut, &len);
                if(ret != 0){
                    code_flag = CODE_FLAGS_QR;
                    break;
                }
            }
            if(mFlags & CODE_FLAGS_BAR){
                ret = this->BARdecode(pframe->pBuff, pframe->width,pframe->height, &pReslut, &len);
                if(ret != 0){
                    code_flag = CODE_FLAGS_BAR;
                    break;
                }
            }
            if(mFlags & CODE_FLAGS_DM){
                ret = this->DMdecode(pframe->pBuff, pframe->width,pframe->height, &pReslut, &len);
                if(ret != 0){
                    code_flag = CODE_FLAGS_DM;
                    break;
                }
            }
            if(mFlags & CODE_FLAGS_HX){
                ret = this->HXdecode(pframe->pBuff, pframe->width,pframe->height, &pReslut, &len);
                if(ret != 0){
                    code_flag = CODE_FLAGS_HX;
                    break;
                }
            }
        }while(0);

        if(ret && (pout_buf != NULL) && (pout_len != NULL)){
            if(*pout_len > len){
                memcpy(pout_buf, pReslut, len);
                *pout_len = len;
            }
        }
        if(ret && (mLisener != NULL)){
            mLisener->onDecode(pReslut, len);
        }
        // free pResult;
        return NO_ERROR;

    }
    IMPLEMENT_DECODER_FUNCTION(Decoder, QRdecode);
    IMPLEMENT_DECODER_FUNCTION(Decoder, HXdecode);
    IMPLEMENT_DECODER_FUNCTION(Decoder, DMdecode);
    IMPLEMENT_DECODER_FUNCTION(Decoder, BARdecode);

    IDecoder* IDecoder::createDecoder(Type_e type, unsigned char flags){
        return new Decoder(type, flags);
    }
    void IDecoder::destroyDecoder(IDecoder* decoder){
        Decoder * self = (Decoder*)decoder;
        delete self;
    }

}
