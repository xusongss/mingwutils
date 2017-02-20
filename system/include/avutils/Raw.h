#ifndef _RAW_H_
#define _RAW_H_
#include<avutils/PixFmts.h>
#ifdef __cplusplus
extern "C" {
#endif

#define MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define MKBETAG(a,b,c,d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))

typedef struct PixelFormatTag {
	enum AVPixelFormat pix_fmt;
	unsigned int fourcc;
}PixelFormatTag_t;
const PixelFormatTag_t * get_raw_pix_fmt_tags(void);

#ifdef __cplusplus
} // extern "C"
#endif
#endif