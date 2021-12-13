/*
 * File: vecnorm.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Jul-2021 11:13:20
 */

/* Include Files */
#include "vecnorm.h"

#include "get_quat_from_K.h"
#include "optimal_request.h"
#include "optimal_request_init.h"

#include <math.h>

/* Function Definitions */

/*
 * Arguments    : const float x[3]
 * Return Type  : float
 */
float vecnorm(const float x[3])
{
    float y;
    float scale;
    float absxk;
    float t;
    scale = 1.29246971E-26F;
    absxk = fabsf(x[0]);
    if (absxk > 1.29246971E-26F) {
        y     = 1.0F;
        scale = absxk;
    } else {
        t = absxk / 1.29246971E-26F;
        y = t * t;
    }

    absxk = fabsf(x[1]);
    if (absxk > scale) {
        t     = scale / absxk;
        y     = 1.0F + y * t * t;
        scale = absxk;
    } else {
        t = absxk / scale;
        y += t * t;
    }

    absxk = fabsf(x[2]);
    if (absxk > scale) {
        t     = scale / absxk;
        y     = 1.0F + y * t * t;
        scale = absxk;
    } else {
        t = absxk / scale;
        y += t * t;
    }

    return scale * sqrtf(y);
}

/*
 * File trailer for vecnorm.c
 *
 * [EOF]
 */
