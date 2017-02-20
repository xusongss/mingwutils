#ifndef _CAR_H_
#define _CAR_H_
#include <utils/Context.h>
namespace Athena {
    class Car : public Context {
    public:
        Car(const char *name);

        virtual ~Car();

    public:
        const char *getName() {
            return mName;
        }

        int setParameter(const char *key, const char *val);

        void display();

    protected:
        config_ctx_t *getCtx();

    private:
        void *mCtx;
        char mName[128];
    };
}
#endif