#define LOG_TAG "queue test"
#include <utils/Queue.h>
#include <utils/thread.h>
#include <stdio.h>
#include <utils/Log.h>
using namespace Athena;
class RevService :public Thread{
public:
    RevService(sp<Queue> q):mQ(q) {}
    ~RevService(){
        ALOGV("~RevService is called ");
    }
    virtual bool        threadLoop(){
        msg_t *msg;
        msg = mQ->dequeue();
        if(msg!= NULL){
            ALOGV("recv a msg %s\n", msg->data);
            free(msg);
        }else{
            ALOGV("time out\n");
            return false;
        }
        return true;
    }
private:
    sp<Queue>mQ;
};
int main()
{
    int i = 0;
    sp<Queue> que = new Queue();
    sp<RevService> th = new RevService(que);
    th->run();
    do{
        int ret = 0;
        printf("input int %d \n", i);
        ret = scanf("%d", &i);
        if(ret == 1){
            msg_t *msg;
            if(i == 10){
                que->notifyAll();
                th->requestExitAndWait();
                break;
            }else{
                msg = (msg_t *)malloc(sizeof(msg_t) + 128);
                sprintf((char*)msg->data, "%d", i);
                que->enqueue(msg, 0);
            }

        }
    }while(1);


}