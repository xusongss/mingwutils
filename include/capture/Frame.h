#ifndef _FRAME_H_
#define _FRAME_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    int fmt;/*AVPixelFormat*/
    int size;
    unsigned char * pBuff;
}Frame_t;
#ifdef __cplusplus
} // extern "C"
#endif
#endif