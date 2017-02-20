#ifndef _IMAGE_DECODER_FUNC_H_
#define _IMAGE_DECODER_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

int ImageQRdecode(unsigned char *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength){
    return 0;
}
int ImageHXdecode(unsigned char *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength){
    return 0;
}
int ImageDMdecode(unsigned char *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength){
    return 0;
}
int ImageBARdecode(unsigned char *GetImage, int iWidth, int iHeight, unsigned char * *pReslut, int *DataLength){
    return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif
#endif