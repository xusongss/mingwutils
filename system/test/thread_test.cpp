#define LOG_TAG "thead_test"
#include<utils/Log.h>
#include<utils/Thread.h>
#include <unistd.h>

using namespace Athena;
class ThreadTest:public Thread{
    virtual ~ThreadTest(){
        ALOGV("~ThreadTest is call ");
    }
    virtual bool  threadLoop(){
        static int i = 10;
        ALOGV("threadLoop");
        if(!(i--)){
            return false;
        }{
            sleep(1);
        }
        return true;
    }
};
int main() {
    sp<ThreadTest> th = new ThreadTest();
    th->run();
    th->join();
    return 0;

}