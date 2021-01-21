#ifndef QUANTIZE_H
#define QUANTIZE_H

/*
 * Quantizes a given block that contains the picture luminance
 * The output array must be initialized beforehand to the correct size (64 elements)
 *
 */
void quantize_luminance(float *block, char *output);

/*
 * Quantizes a given block that contains the picture chrominance
 * The output array must be initialized beforehand to the correct size (64 elements)
 *
 */
void quantize_chrominance(float *block, char *output);

#endif

