#include "zigzag.h"
#define UP 0
#define DOWN 1

void zigzag(char *block)
{

	char temp_field[64] = {0};
	int col = 0, row = 0, counter = 0;
	int direction = UP, at_edge = 0;

	while (1) {
		temp_field[counter++] = block[row * 8 + col];

		if (col == 7 && row == 7) {
			break;
		}

		if(!at_edge) {
			if (row == 0 || col == 7) {
				direction = DOWN;
				at_edge = 1;

				if (row == 0) {
					col++;
				} else {
					row++;
				}

				continue;
			}

			if (row == 7 || col == 0) {
				direction = UP;
				at_edge = 1;

				if (row == 7) {
					col++;
				} else {
					row++;
				}

				continue;
			}
		}

		if (direction == UP) {
			row--;
			col++;
		} else {
			row++;
			col--;
		}
		at_edge = 0;
	}

	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			block[row * 8 + col] = temp_field[row * 8 + col];
		}
	}
}
