#ifndef DEQUANTIZE_H
#define DEQUANTIZE_H

/*
 * Dequantizes a given block that contains the picture luminance
 * The output array must be initialized beforehand to the correct size (64 elements)
 *
 */
void dequantize_luminance(char *block, float *output);

/*
 * Dequantizes a given block that contains the picture chrominance
 * The output array must be initialized beforehand to the correct size (64 elements)
 *
 */
void dequantize_chrominance(char *block, float *output);

#endif

