#include<utils/RWLock.h>
#include <pthread.h>
namespace Athena {
     RWLock::RWLock() {
        pthread_rwlock_init(&mRWLock, NULL);
    }

     RWLock::RWLock(__attribute__((unused)) const char *name) {
        pthread_rwlock_init(&mRWLock, NULL);
    }

     RWLock::RWLock(int type, __attribute__((unused)) const char *name) {
        if (type == SHARED) {
            pthread_rwlockattr_t attr;
            pthread_rwlockattr_init(&attr);
            pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
            pthread_rwlock_init(&mRWLock, &attr);
            pthread_rwlockattr_destroy(&attr);
        } else {
            pthread_rwlock_init(&mRWLock, NULL);
        }
    }

     RWLock::~RWLock() {
        pthread_rwlock_destroy(&mRWLock);
    }

     status_t RWLock::readLock() {
        return -pthread_rwlock_rdlock(&mRWLock);
    }

     status_t RWLock::tryReadLock() {
        return -pthread_rwlock_tryrdlock(&mRWLock);
    }

     status_t RWLock::writeLock() {
        return -pthread_rwlock_wrlock(&mRWLock);
    }

     status_t RWLock::tryWriteLock() {
        return -pthread_rwlock_trywrlock(&mRWLock);
    }

    void RWLock::unlock() {
        pthread_rwlock_unlock(&mRWLock);
    }
}