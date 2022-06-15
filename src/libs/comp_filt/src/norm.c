/*
 * File: norm.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 15-Jun-2022 11:53:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "comp_filt_init.h"
#include "comp_filt_step.h"
#include "norm.h"

/* Function Definitions */

/*
 * Arguments    : const float x[4]
 * Return Type  : float
 */
float b_norm(const float x[4])
{
  float y;
  float scale;
  int k;
  float absxk;
  float t;
  y = 0.0F;
  scale = 1.17549435E-38F;
  for (k = 0; k < 4; k++) {
    absxk = fabsf(x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0F + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrtf(y);
}

/*
 * Arguments    : const float x[3]
 * Return Type  : float
 */
float norm(const float x[3])
{
  float y;
  float scale;
  int k;
  float absxk;
  float t;
  y = 0.0F;
  scale = 1.17549435E-38F;
  for (k = 0; k < 3; k++) {
    absxk = fabsf(x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0F + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrtf(y);
}

/*
 * File trailer for norm.c
 *
 * [EOF]
 */
