#include <stdlib.h>

#include "decoder.h"

#include "dequantize.h"
#include "dezigzag.h"
#include "dezrl.h"
#include "idct.h"

__declspec(dllexport) void decode(unsigned char *buffer, unsigned char *output)
{

	char output_block_order[64];

	int i = 0;
	for(int j = 0; j < 4800; j++)
	{
		int k = 0;
		while(buffer[i] != 0x7f)
		{
			output_block_order[k] = buffer[i];
			i++;
			k++;
		}
		output_block_order[k] = buffer[i];
		i++;
		dezrl(output_block_order);
		dezigzag(output_block_order);
		float fblock[64];
		dequantize_luminance(output_block_order, fblock);
		dctInverse(fblock);
		for(int i = 0; i < 64; i++)
		{
			fblock[i] += 128;
		}
		int block_x = j % 80;
		int block_y = j / 80;
		for(int y = 0; y < 8; y++)
		{
			for(int x = 0; x < 8; x++)
			{
				output[block_y * 640 * 8 + block_x * 8 + y * 640 + x] = fblock[x + y * 8];
			}
		}
	}

	output += 640 * 480;

	for(int j = 0; j < 1200; j++)
	{
		int k = 0;
		while(buffer[i] != 0x7f)
		{
			output_block_order[k] = buffer[i];
			i++;
			k++;
		}
		output_block_order[k] = buffer[i];
		i++;
		dezrl(output_block_order);
		dezigzag(output_block_order);
		float fblock[64];
		dequantize_chrominance(output_block_order, fblock);
		dctInverse(fblock);
		for(int i = 0; i < 64; i++)
		{
			fblock[i] += 128;
		}
		int block_x = j % 40;
		int block_y = j / 40;
		for(int y = 0; y < 8; y++)
		{
			for(int x = 0; x < 8; x++)
			{
				output[block_y * 320 * 8 + block_x * 8 + y * 320 + x] = fblock[x * 8 + y];
			}
		}
	}

	output += 320 * 240;

	for(int j = 0; j < 1200; j++)
	{
		int k = 0;
		while(buffer[i] != 0x7f)
		{
			output_block_order[k] = buffer[i];
			i++;
			k++;
		}
		output_block_order[k] = buffer[i];
		i++;
		dezrl(output_block_order);
		dezigzag(output_block_order);
		float fblock[64];
		dequantize_chrominance(output_block_order, fblock);
		dctInverse(fblock);
		for(int i = 0; i < 64; i++)
		{
			fblock[i] += 128;
		}
		int block_x = j % 40;
		int block_y = j / 40;
		for(int y = 0; y < 8; y++)
		{
			for(int x = 0; x < 8; x++)
			{
				output[block_y * 320 * 8 + block_x * 8 + y * 320 + x] = fblock[x * 8 + y];
			}
		}
	}

}

































