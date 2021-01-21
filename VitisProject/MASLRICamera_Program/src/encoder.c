
#include "encoder.h"
#include "dct.h"
#include "quantize.h"
#include "zigzag.h"
#include "zrl.h"

#include <stdlib.h>
#include <string.h>
#include <xil_printf.h>

#define BLOCKCOUNT 4800
#define WIDTH 640
#define HEIGHT 480
#define BLOCKWIDTH 8
#define BLOCKHEIGHT 8

static void fetch_and_prep_for_dct(unsigned char *blocks, int block_num, float *output, int mode) {

    int position = 0;

    // find row
    position = (WIDTH/mode) * BLOCKHEIGHT * (block_num / ((WIDTH/mode) / BLOCKWIDTH));

    // find col
    position += BLOCKWIDTH * (block_num % ((WIDTH/mode) / BLOCKWIDTH));

    // shift and copy block
    for (int i = 0; i < BLOCKHEIGHT; i++) {
        for (int j = 0; j < BLOCKWIDTH; j++) {
            output[i * BLOCKWIDTH + j] = (float)blocks[position++] - 128;
        }
        position += (WIDTH/mode) - BLOCKWIDTH;
    }
}

int encode(unsigned char *Y, unsigned char *U, unsigned char *V, unsigned char **output)  {

    float dct_block[BLOCKWIDTH * BLOCKHEIGHT] = {0};
    char component_block[BLOCKWIDTH * BLOCKHEIGHT] = {0};

    char y_buff[WIDTH * HEIGHT + 4800] = {0};
    char u_buff[WIDTH * HEIGHT / 4 + 1200] = {0};
    char v_buff[WIDTH * HEIGHT / 4 + 1200] = {0};
    int zrl_size = 0;
    int component_buff_size = 0;

    int zrl_y_size = 0;
    int zrl_u_size = 0;
    int zrl_v_size = 0;

    unsigned char *encoded_image;

    // processing Y component
    for(int i = 0; i < BLOCKCOUNT; i++) {
        fetch_and_prep_for_dct(Y, i, dct_block, 1);
        dct(dct_block);
        quantize_luminance(dct_block, component_block);
        zigzag(component_block);
        zrl_size = zrl(component_block);
        memcpy(y_buff + component_buff_size, component_block, zrl_size);
        component_buff_size += zrl_size;
    }

    zrl_y_size = component_buff_size;
    xil_printf("Encoded Y\n");

    component_buff_size = 0;

    // processing U component
    for(int i = 0; i < BLOCKCOUNT/4; i++) {
        fetch_and_prep_for_dct(U, i, dct_block, 2);
        dct(dct_block);
        quantize_chrominance(dct_block, component_block);
        zigzag(component_block);
        zrl_size = zrl(component_block);
        memcpy(u_buff + component_buff_size, component_block, zrl_size);
        component_buff_size += zrl_size;
    }

    zrl_u_size = component_buff_size;
    xil_printf("Encoded U\n");

    component_buff_size = 0;

    // processing V component
    for(int i = 0; i < BLOCKCOUNT/4; i++) {
        fetch_and_prep_for_dct(V, i, dct_block, 2);
        dct(dct_block);
        quantize_chrominance(dct_block, component_block);
        zigzag(component_block);
        zrl_size = zrl(component_block);
        memcpy(v_buff + component_buff_size, component_block, zrl_size);
        component_buff_size += zrl_size;
    }

    zrl_v_size = component_buff_size;
    xil_printf("Encoded V\n");

    encoded_image = malloc(zrl_y_size + zrl_u_size + zrl_v_size);

    // write all encoded components to buffer
    memcpy(encoded_image, y_buff, zrl_y_size);
    memcpy(encoded_image + zrl_y_size, u_buff, zrl_u_size);
    memcpy(encoded_image + zrl_y_size + zrl_u_size, v_buff, zrl_v_size);

    *output = encoded_image;

    return zrl_y_size + zrl_u_size + zrl_v_size;
}
