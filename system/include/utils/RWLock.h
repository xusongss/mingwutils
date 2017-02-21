/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LIBS_UTILS_RWLOCK_H
#define _LIBS_UTILS_RWLOCK_H

#include <stdint.h>
#include <sys/types.h>
#include <utils/Errors.h>
#include <utils/ThreadDefs.h>

#if defined(HAVE_PTHREADS)
# include <pthread.h>
#endif

// ---------------------------------------------------------------------------
namespace Athena {
// ---------------------------------------------------------------------------


/*
 * Simple mutex class.  The implementation is system-dependent.
 *
 * The mutex must be unlocked by the thread that locked it.  They are not
 * recursive, i.e. the same thread can't lock it multiple times.
 */
class RWLock {
public:
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

                RWLock();
                RWLock(const char* name);
                RWLock(int type, const char* name = NULL);
                ~RWLock();

    status_t    readLock();
    status_t    tryReadLock();
    status_t    writeLock();
    status_t    tryWriteLock();
    void        unlock();

    class AutoRLock {
    public:
        inline AutoRLock(RWLock& rwlock) : mLock(rwlock)  { mLock.readLock(); }
        inline ~AutoRLock() { mLock.unlock(); }
    private:
        RWLock& mLock;
    };

    class AutoWLock {
    public:
        inline AutoWLock(RWLock& rwlock) : mLock(rwlock)  { mLock.writeLock(); }
        inline ~AutoWLock() { mLock.unlock(); }
    private:
        RWLock& mLock;
    };

private:
    // A RWLock cannot be copied
                RWLock(const RWLock&);
   RWLock&      operator = (const RWLock&);

    pthread_rwlock_t mRWLock;
};
// ---------------------------------------------------------------------------
}; // namespace android
// ---------------------------------------------------------------------------

#endif // _LIBS_UTILS_RWLOCK_H
