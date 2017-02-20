#include <stdarg.h>
#include<utils/Log.h>
#include <cstdio>
#include <string.h>
#include <stdio.h>

#define LOG_BUF_SIZE	1024
static const char * _log_title[]={
        "UNKNOWN",
        "DEFAULT",
        "VERBOSE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL",
        "SILENT",
        "NULL"
};
int __android_log_write(int prio, const char *tag, const char *text){
    printf("[ %s ] [%s] %s\r\n", _log_title[prio], tag, text);
}
int __android_log_print(int prio, const char *tag,  const char *fmt, ...){
    va_list ap;
    char buf[LOG_BUF_SIZE];

    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    return __android_log_write(prio, tag, buf);
}
int __android_log_vprint(int prio, const char *tag, const char *fmt, va_list ap){

    char buf[LOG_BUF_SIZE];

    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);

    return __android_log_write(prio, tag, buf);
}
void __android_log_assert(const char *cond, const char *tag, const char *fmt, ...){

    char buf[LOG_BUF_SIZE];

    if (fmt) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
        va_end(ap);
    } else {
        /* Msg not provided, log condition.  N.B. Do not use cond directly as
         * format string as it could contain spurious '%' syntax (e.g.
         * "%d" in "blocks%devs == 0").
         */
        if (cond)
            snprintf(buf, LOG_BUF_SIZE, "Assertion failed: %s", cond);
        else
            strcpy(buf, "Unspecified assertion failed");
    }

    __android_log_write(LOG_FATAL, tag, buf);

#if defined(__GNUC__)
    __builtin_trap();
#endif
}
