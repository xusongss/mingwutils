#ifndef _PIX_DESC_H_
#define _PIX_DESC_H_
#include <inttypes.h>
#include<avutils/PixFmts.h>
#ifdef __cplusplus
extern "C" {
#endif

const char * get_pix_fmt_name(enum AVPixelFormat pix_fmt);


#if defined(_MSC_VER)
#    define attribute_deprecated __declspec(deprecated)
#else
#    define attribute_deprecated
#endif
typedef struct AVComponentDescriptor {
	/**
	* Which of the 4 planes contains the component.
	*/
	int plane;

	/**
	* Number of elements between 2 horizontally consecutive pixels.
	* Elements are bits for bitstream formats, bytes otherwise.
	*/
	int step;

	/**
	* Number of elements before the component of the first pixel.
	* Elements are bits for bitstream formats, bytes otherwise.
	*/
	int offset;

	/**
	* Number of least significant bits that must be shifted away
	* to get the value.
	*/
	int shift;

	/**
	* Number of bits in the component.
	*/
	int depth;


	/** deprecated, use step instead */
	attribute_deprecated int step_minus1;

	/** deprecated, use depth instead */
	attribute_deprecated int depth_minus1;

	/** deprecated, use offset instead */
	attribute_deprecated int offset_plus1;

} AVComponentDescriptor;

/**
* Descriptor that unambiguously describes how the bits of a pixel are
* stored in the up to 4 data planes of an image. It also stores the
* subsampling factors and number of components.
*
* @note This is separate of the colorspace (RGB, YCbCr, YPbPr, JPEG-style YUV
*       and all the YUV variants) AVPixFmtDescriptor just stores how values
*       are stored not what these values represent.
*/
typedef struct AVPixFmtDescriptor {
	const char *name;
	uint8_t nb_components;  ///< The number of components each pixel has, (1-4)

	/**
	* Amount to shift the luma width right to find the chroma width.
	* For YV12 this is 1 for example.
	* chroma_width = AV_CEIL_RSHIFT(luma_width, log2_chroma_w)
	* The note above is needed to ensure rounding up.
	* This value only refers to the chroma components.
	*/
	uint8_t log2_chroma_w;

	/**
	* Amount to shift the luma height right to find the chroma height.
	* For YV12 this is 1 for example.
	* chroma_height= AV_CEIL_RSHIFT(luma_height, log2_chroma_h)
	* The note above is needed to ensure rounding up.
	* This value only refers to the chroma components.
	*/
	uint8_t log2_chroma_h;

	/**
	* Combination of AV_PIX_FMT_FLAG_... flags.
	*/
	uint64_t flags;

	/**
	* Parameters that describe how pixels are packed.
	* If the format has 1 or 2 components, then luma is 0.
	* If the format has 3 or 4 components:
	*   if the RGB flag is set then 0 is red, 1 is green and 2 is blue;
	*   otherwise 0 is luma, 1 is chroma-U and 2 is chroma-V.
	*
	* If present, the Alpha channel is always the last component.
	*/
	AVComponentDescriptor comp[4];

	/**
	* Alternative comma-separated names.
	*/
	const char *alias;
} AVPixFmtDescriptor;
/**
* Pixel format is big-endian.
*/
#define AV_PIX_FMT_FLAG_BE           (1 << 0)
/**
* Pixel format has a palette in data[1], values are indexes in this palette.
*/
#define AV_PIX_FMT_FLAG_PAL          (1 << 1)
/**
* All values of a component are bit-wise packed end to end.
*/
#define AV_PIX_FMT_FLAG_BITSTREAM    (1 << 2)
/**
* Pixel format is an HW accelerated format.
*/
#define AV_PIX_FMT_FLAG_HWACCEL      (1 << 3)
/**
* At least one pixel component is not in the first data plane.
*/
#define AV_PIX_FMT_FLAG_PLANAR       (1 << 4)
/**
* The pixel format contains RGB-like data (as opposed to YUV/grayscale).
*/
#define AV_PIX_FMT_FLAG_RGB          (1 << 5)

/**
* The pixel format is "pseudo-paletted". This means that it contains a
* fixed palette in the 2nd plane but the palette is fixed/constant for each
* PIX_FMT. This allows interpreting the data as if it was PAL8, which can
* in some cases be simpler. Or the data can be interpreted purely based on
* the pixel format without using the palette.
* An example of a pseudo-paletted format is AV_PIX_FMT_GRAY8
*/
#define AV_PIX_FMT_FLAG_PSEUDOPAL    (1 << 6)

/**
* The pixel format has an alpha channel. This is set on all formats that
* support alpha in some way. The exception is AV_PIX_FMT_PAL8, which can
* carry alpha as part of the palette. Details are explained in the
* AVPixelFormat enum, and are also encoded in the corresponding
* AVPixFmtDescriptor.
*
* The alpha is always straight, never pre-multiplied.
*
* If a codec or a filter does not support alpha, it should set all alpha to
* opaque, or use the equivalent pixel formats without alpha component, e.g.
* AV_PIX_FMT_RGB0 (or AV_PIX_FMT_RGB24 etc.) instead of AV_PIX_FMT_RGBA.
*/
#define AV_PIX_FMT_FLAG_ALPHA        (1 << 7)

#ifdef __cplusplus
} // extern "C"
#endif
#endif