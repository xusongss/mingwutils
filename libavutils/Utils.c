#include <avutils/Utils.h>
AVPixelFormat_e find_pix_fmt(const PixelFormatTag_t *tags,
	unsigned int fourcc)
{
	while (tags->pix_fmt >= 0) {
		if (tags->fourcc == fourcc)
			return tags->pix_fmt;
		tags++;
	}
	return AV_PIX_FMT_NONE;
}