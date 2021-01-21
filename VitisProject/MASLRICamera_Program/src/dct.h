#ifndef DCT_H
#define DCT_H

/*
 * Function: dctBlock
 * ----------------------------
 *   calculus DCT for matrix 8*8 using dct1d function
 *
 *   block: float *
 *
 *   returns: DCT (modified block)
 */
void dct(float *block);

#endif //DCT_H
