#ifndef _UTILS_H_
#define _UTILS_H_
#include <avutils/PixFmts.h>
#include <avutils/Raw.h>

#ifdef __cplusplus
extern "C" {
#endif

AVPixelFormat_e find_pix_fmt(const PixelFormatTag_t *tags, unsigned int fourcc);

#ifdef __cplusplus
} // extern "C"
#endif
#endif