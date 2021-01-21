#include "dct.h"
#include <math.h>
/*
 * Function: dct1d
 * ----------------------------
 *   KR algorithm for scaled 1D DCT (8 points)
 *
 *   vector: float *
 *
 *   returns: scaled DCT (modified vector)
 */
static void dct1d(float *vector);

static const float SCALE[] = {
	0.353553390593273762200422,
	0.254897789552079584470970,
	0.270598050073098492199862,
	0.300672443467522640271861,
	0.353553390593273762200422,
	0.449988111568207852319255,
	0.653281482438188263928322,
	1.281457723870753089398043
};

static const float A[] = {
	0.707106781186547524400844,
	0.541196100146196984399723,
	0.707106781186547524400844,
	1.306562964876376527856643,
	0.382683432365089771728460
};

void dct(float *block)
{
    // DCT: rows
    for (int offset = 0; offset < 8; offset++)
        dct1d(block + offset * 8);

    // DCT: columns
    for (int offset = 0; offset < 8; offset++)
    {
        float column[8];
        for(int i = 0; i < 8; i++)
            column[i] = *(block + offset + i * 8);
        dct1d(column);
        for(int i = 0; i < 8; i++)
            *(block + offset + i * 8) = column[i];
    }
}

static void dct1d(float *vector)
{
	const float v0 = vector[0] + vector[7];
		const float v1 = vector[1] + vector[6];
		const float v2 = vector[2] + vector[5];
		const float v3 = vector[3] + vector[4];
		const float v4 = vector[3] - vector[4];
		const float v5 = vector[2] - vector[5];
		const float v6 = vector[1] - vector[6];
		const float v7 = vector[0] - vector[7];

		const float v8 = v0 + v3;
		const float v9 = v1 + v2;
		const float v10 = v1 - v2;
		const float v11 = v0 - v3;
		const float v12 = -v4 - v5;
		const float v13 = (v5 + v6) * A[2];
		const float v14 = v6 + v7;

		const float v15 = v8 + v9;
		const float v16 = v8 - v9;
		const float v17 = (v10 + v11) * A[0];
		const float v18 = (v12 + v14) * A[4];

		const float v19 = -v12 * A[1] - v18;
		const float v20 = v14 * A[3] - v18;

		const float v21 = v17 + v11;
		const float v22 = v11 - v17;
		const float v23 = v13 + v7;
		const float v24 = v7 - v13;

		const float v25 = v19 + v24;
		const float v26 = v23 + v20;
		const float v27 = v23 - v20;
		const float v28 = v24 - v19;

		vector[0] = SCALE[0] * v15;
		vector[1] = SCALE[1] * v26;
		vector[2] = SCALE[2] * v21;
		vector[3] = SCALE[3] * v28;
		vector[4] = SCALE[4] * v16;
		vector[5] = SCALE[5] * v25;
		vector[6] = SCALE[6] * v22;
		vector[7] = SCALE[7] * v27;
}
