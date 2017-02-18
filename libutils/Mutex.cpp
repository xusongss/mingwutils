#include<utils/config.h>
#include<utils/Mutex.h>
# include <pthread.h>
namespace Athena {
    Mutex::Mutex() {
        pthread_mutex_init(&mMutex, NULL);
    }
    Mutex::Mutex(__attribute__((unused)) const char* name) {
        pthread_mutex_init(&mMutex, NULL);
    }
    Mutex::Mutex(int type, __attribute__((unused)) const char* name) {
        if (type == SHARED) {
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
            pthread_mutex_init(&mMutex, &attr);
            pthread_mutexattr_destroy(&attr);
        } else {
            pthread_mutex_init(&mMutex, NULL);
        }
    }
    Mutex::~Mutex() {
        pthread_mutex_destroy(&mMutex);
    }
    status_t Mutex::lock() {
        return -pthread_mutex_lock(&mMutex);
    }
    void Mutex::unlock() {
        pthread_mutex_unlock(&mMutex);
    }
    status_t Mutex::tryLock() {
        return -pthread_mutex_trylock(&mMutex);
    }
}