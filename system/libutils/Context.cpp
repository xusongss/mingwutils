#define LOG_TAG "Context"
#define NDEBUG
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils/Context.h>
#include <utils/Log.h>
#include <utils/Errors.h>
namespace Athena{
static status_t config_default(config_ctx_t *ctx){
    if(ctx == NULL || ctx->ops == NULL){
        return INVALID_OPERATION;
    }
    ALOGV("%s", "set default values:");
    const Option_t *ins = ctx->ops;
    while(ins->name != NULL){
        void *dst = ((uint8_t *)ctx) + ins->offset;
        switch (ins->type){
            case OPT_TYPE_INT:
            {
                    *(int *)dst = ins->default_val.i64;
                    ALOGV(" name (%s) default (%d)", ins->name, *(int *)dst);
            }
                break;
            case OPT_TYPE_INT64:
            {
                    *(int64_t *)dst = ins->default_val.i64;
                    ALOGV(" name (%s) default (%ld)", ins->name, *(int64_t *)dst);
            }
                break;
            case OPT_TYPE_STRING:
            {
                if(ins->default_val.str == NULL){
                    *(char **)dst=NULL;
                    ALOGV(" name (%s) default (null)", ins->name);
                }else{
                    *(char **)dst = strdup(ins->default_val.str);
                    ALOGV(" name (%s) default (%s)", ins->name,*(char **)dst);
                }
            }
                break;
            default:
                ALOGE(" name (%s) not support type", ins->name);
                break;
        }
        ins++;
    }
    return NO_ERROR;
}
static status_t config_set(config_ctx_t *ctx,const char * key, const char * val, int search_flags){
    const Option_t *ins = ctx->ops;
    if(ctx == NULL || ins == NULL){
        return -1;
    }
    int find = 0;
    int notify = 0;
    while (ins->name != NULL){
        void *dst = ((uint8_t *)ctx) + ins->offset;
        if (!strcmp(ins->name, key)){
            find = 1;
            switch (ins->type){
                case OPT_TYPE_INT:
                {
                    int num;
                    int old = *(int *)dst;
                    if (sscanf(val, "%d", &num) == 1){
                        if((ins->min <=num) && (num <= ins->max) ){
                            *(int *)dst = num;
                            notify = old == num ? 0 : 1;
                            ALOGV("name (%s) val (%d)", ins->name, *(int *)dst);
                        }else{
                            ALOGE("name (%s) val (%d) bad value", ins->name, num );
                            return BAD_VALUE;
                        }

                    }
                }
                    break;
                case OPT_TYPE_INT64:
                {
                    int64_t num;
                    int64_t old = *(int64_t *) dst;
                    if (sscanf(val, "%d", &num) == 1){
                        if((ins->min <=num) && (num <= ins->max) ) {
                            *(int64_t *) dst = num;
                            notify = old == num ? 0 : 1;
                            ALOGV("name (%s) val (%ld)", ins->name, *(int64_t *)dst);
                        }else{
                            ALOGE("name (%s) val (%dl) bad value", ins->name, num );
                            return BAD_VALUE;
                        }
                    }
                }
                    break;
                case OPT_TYPE_STRING:
                {
                    char * old = *(char **)dst;
                    *(char **)dst = strdup(val);
                    ALOGV("name (%s) val (%s)", ins->name,*(char **)dst);
                    if(old != NULL){
                        notify = !strcmp(old, val) ? 0 : 1;
                        free(old);
                    }
                }
                    break;
                default:
                    ALOGE("name (%s) not support type", ins->name);
                    return NO_SUPPORTED;
            }
        }
        if(find && notify){
            ins->notify(ctx, key, val);
            break;
        }
        ins++;
    }
    if(!find){
        ALOGE("not support key %s val %s", key, val);
        return NO_SUPPORTED;
    }

    return NO_ERROR;
}
    Context::Context(){
    }
    Context::~Context(){
    }
    status_t Context::setDefault(){
        config_default(this->getCtx());
    }
    status_t Context::set(const char * key, const char * val, int search_flags){
        config_ctx_t *ctx = this->getCtx();
        if(ctx != NULL){
            return config_set(ctx, key, val, search_flags);
        }
    }
}