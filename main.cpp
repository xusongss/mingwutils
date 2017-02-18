#define LOG_TAG "cccc"
#include <iostream>
#include<utils/Log.h>
#include<utils/Thread.h>
using namespace Athena;
class ThreadTest:public Thread{
    virtual ~ThreadTest(){
        ALOGV("dest");
    }
    virtual bool        threadLoop(){
        static int i = 10;
        ALOGV("threadLoop");
        if(i--){
            return false;
        }
        return true;
    }
};
int main() {
    std::cout << "Hello, World!" << std::endl;
    ALOGV("MAIN");
    ALOGV("MAIN");
    ALOGV("MAIN");
    ALOGV("MAIN");
    sp<ThreadTest> th = new ThreadTest();
    th->run();
    th->join();
    return 0;

}