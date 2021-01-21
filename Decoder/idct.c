#include "idct.h"
#include <math.h>

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

static void dct1dInverse(float *vector);

void dctInverse(float *block)
{

    // DCT: rows
    for (int offset = 0; offset < 8; offset++)
        dct1dInverse(block + offset * 8);

    // DCT: columns
    for (int offset = 0; offset < 8; offset++)
    {
        float column[8];
        for(int i = 0; i < 8; i++)
            column[i] = *(block + offset + i * 8);
        dct1dInverse(column);
        for(int i = 0; i < 8; i++)
            *(block + offset + i * 8) = column[i];
    }
}

static void dct1dInverse(float *vector)
{
	const float v15 = vector[0] / SCALE[0];
	const float v26 = vector[1] / SCALE[1];
	const float v21 = vector[2] / SCALE[2];
	const float v28 = vector[3] / SCALE[3];
	const float v16 = vector[4] / SCALE[4];
	const float v25 = vector[5] / SCALE[5];
	const float v22 = vector[6] / SCALE[6];
	const float v27 = vector[7] / SCALE[7];
	
	const float v19 = (v25 - v28) / 2;
	const float v20 = (v26 - v27) / 2;
	const float v23 = (v26 + v27) / 2;
	const float v24 = (v25 + v28) / 2;
	
	const float v7  = (v23 + v24) / 2;
	const float v11 = (v21 + v22) / 2;
	const float v13 = (v23 - v24) / 2;
	const float v17 = (v21 - v22) / 2;
	
	const float v8 = (v15 + v16) / 2;
	const float v9 = (v15 - v16) / 2;
	
	const float v18 = (v19 - v20) * A[4];
	const float v12 = (v19 * A[3] - v18) / (A[1] * A[4] - A[1] * A[3] - A[3] * A[4]);
	const float v14 = (v18 - v20 * A[1]) / (A[1] * A[4] - A[1] * A[3] - A[3] * A[4]);
	
	const float v6 = v14 - v7;
	const float v5 = v13 / A[2] - v6;
	const float v4 = -v5 - v12;
	const float v10 = v17 / A[0] - v11;
	
	const float v0 = (v8 + v11) / 2;
	const float v1 = (v9 + v10) / 2;
	const float v2 = (v9 - v10) / 2;
	const float v3 = (v8 - v11) / 2;
	
	vector[0] = (v0 + v7) / 2;
	vector[1] = (v1 + v6) / 2;
	vector[2] = (v2 + v5) / 2;
	vector[3] = (v3 + v4) / 2;
	vector[4] = (v3 - v4) / 2;
	vector[5] = (v2 - v5) / 2;
	vector[6] = (v1 - v6) / 2;
	vector[7] = (v0 - v7) / 2;
}