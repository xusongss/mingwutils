#include <utils/Queue.h>
namespace Athena{
    static void list_init(struct node *node)
    {
        node->next = node;
        node->prev = node;
    }
    static void list_insert_last(struct node *node, struct node *head)
    {
        node->prev = head->prev;
        node->next = head;
        head->prev->next = node;
        head->prev = node;
    }
    static int is_empty(struct node *head)
    {
        return head->next == head;
    }
    static node_t *list_get_first(struct node *head)
    {
        struct node *node;
        if (is_empty(head))
            return NULL;

        node = head->next;
        head->next = node->next;
        node->next->prev = head;
        node->next = node->prev = NULL;

        return node;
    }
    Queue::Queue(){
        list_init(&this->head);
        this->count = 0;
    }
    Queue::~Queue(){
    }
    int Queue::enqueue(msg_t* pMsg,int timeout){
        AutoMutex l(this->mMutex);
        list_insert_last(&pMsg->node, &this->head);
        if (this->count == 0){
            this->mCond.signal();
        }
        this->count++;
        return 0;
    }
    msg_t * Queue::dequeue(int timeout){
        node_t *node;
        msg_t *p_msg = NULL;

        AutoMutex l(this->mMutex);

        node = list_get_first(&this->head);
        if (node != NULL) {
            p_msg = (msg_t *)node;
            this->count--;
        } else{
            status_t ret;
            if (timeout > 0){
                ret = this->mCond.waitRelative(this->mMutex, timeout);
            }
            else{
                ret = this->mCond.wait(this->mMutex);
            }
            if (ret != -1){
                node = list_get_first(&this->head);
                p_msg = (msg_t *)node;
                this->count--;
            }
        }
        return p_msg;
    }
    void Queue::notifyAll(){
        this->mCond.broadcast();
    }
}