#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <utils/RefBase.h>
#include <utils/Mutex.h>
#include <utils/Condition.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct node {
    struct node *prev;
    struct node *next;
}node_t;

typedef struct msg {
    struct node node;
    long len;
    unsigned char   reserve[16];
    unsigned char data[0];
}msg_t;

#ifdef __cplusplus
} // extern "C"
#endif
namespace Athena{
    class Queue :public RefBase{
    public:
        Queue();
        ~Queue();
        int  enqueue(msg_t* pMsg,int timeout = -1);
        msg_t * dequeue(int timeout = -1);
        void notifyAll();
    private:
       Mutex mMutex;
        Condition mCond;
        node_t head;
        int count;

        DISALLOW_EVIL_CONSTRUCTORS(Queue);
    };
};
#endif
