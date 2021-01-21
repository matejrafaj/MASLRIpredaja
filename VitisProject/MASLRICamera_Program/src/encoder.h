#ifndef CODER_ENCODER_H
#define CODER_ENCODER_H

/*
 * Performs image encoding
 * The function returns the size of the encoded image.
 * The encoded image itself is returned through the "output" pointer
 * The encoded image is dynamically allocated and must be freed after use!
 */

int encode(unsigned char *Y, unsigned char *U, unsigned char *V, unsigned char **output);

#endif //CODER_ENCODER_H
