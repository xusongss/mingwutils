#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#include<stdint.h>
#include <utils/Errors.h>
namespace Athena{
    struct config_ctx;
    typedef struct config_ctx config_ctx_t;
    typedef void (*notify_f)(config_ctx_t *ctx, const char * key, const char * val);
    class Context{
    public:
        Context();
        virtual ~Context();
    protected:
        status_t setDefault();
        status_t set(const char * key, const char * val, int search_flags =0);
    protected:
        virtual config_ctx_t* getCtx() {return NULL;};
    };

    typedef enum {
        OPT_TYPE_INT,
        OPT_TYPE_INT64,
        OPT_TYPE_STRING

    }KeyType_t;

    typedef struct {
        const char *name;

        /**
        * short English help text
        * @todo What about other languages?
        */
        const char *help;
        /**
        * The offset relative to the context structure where the option
        * value is stored. It should be 0 for named constants.
        */
        int offset;
        KeyType_t type;

        /**
        * the default value for scalar options
        */
        /**
     * the default value for scalar options
     */
        union {
            int64_t i64;
            double dbl;
            const char *str;
        } default_val;
        double min;                 ///< minimum valid value for the option
        double max;                 ///< maximum valid value for the option

        int flags;
        const char *unit;

        notify_f notify;
    }Option_t;

    struct config_ctx {
        Option_t *ops;
    };



}

#endif