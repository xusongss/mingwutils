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
    const Option_t *ins = ctx->ops;
    while(ins->name != NULL){
        void *dst = ((uint8_t *)ctx) + ins->offset;
        switch (ins->type){
            case OPT_TYPE_INT:
            {
                    *(int *)dst = ins->default_val.i64;
            }
                break;
            case OPT_TYPE_INT64:
            {
                    *(int64_t *)dst = ins->default_val.i64;
            }
                break;
            case OPT_TYPE_STRING:
            {
                *(char **)dst = _strdup(ins->default_val.str);
            }
                break;
            default:
                ALOGE("not support type");
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
    while (ins->name != NULL){
        void *dst = ((uint8_t *)ctx) + ins->offset;
        if (!strcmp(ins->name, key)){
            find = 1;
            switch (ins->type){
                case OPT_TYPE_INT:
                {
                    int num;
                    if (sscanf_s(val, "%d", &num) == 1){
                        if((ins->min <=num) && (num <= ins->max) ){
                            *(int *)dst = num;
                        }else{
                            return BAD_VALUE;
                        }

                    }
                }
                    break;
                case OPT_TYPE_INT64:
                {
                    int64_t num;
                    if (sscanf_s(val, "%d", &num) == 1){
                        if((ins->min <=num) && (num <= ins->max) ) {
                            *(int64_t *) dst = num;
                        }else{
                            return BAD_VALUE;
                        }
                    }
                }
                    break;
                case OPT_TYPE_STRING:
                {
                    *(char **)dst = _strdup(val);
                }
                    break;
                default:
                    ALOGE("not support type");
                    return NO_SUPPORTED;
            }
        }
        if(find){
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