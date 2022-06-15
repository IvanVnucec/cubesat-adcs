/*
 * File: comp_filt_init.c
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
 * COMP_FILT_INIT Summary of this function goes here
 *    Detailed explanation goes here
 * Arguments    : const float acc_i[3]
 *                const float mag_i[3]
 *                float fs
 *                float K_cf
 *                float dJ_max
 *                float N_max
 *                float alpha
 *                struct0_T *s
 * Return Type  : void
 */
void comp_filt_init(const float acc_i[3], const float mag_i[3], float fs, float
                    K_cf, float dJ_max, float N_max, float alpha, struct0_T *s)
{
  float value;
  float b_value;
  int i;

  /* 'comp_filt_init:5' s.acc_i = acc_i / norm(acc_i); */
  value = norm(acc_i);

  /* 'comp_filt_init:6' s.mag_i = mag_i / norm(mag_i); */
  b_value = norm(mag_i);
  for (i = 0; i < 3; i++) {
    s->acc_i[i] = acc_i[i] / value;
    s->mag_i[i] = mag_i[i] / b_value;
  }

  /* 'comp_filt_init:7' s.fs = fs; */
  s->fs = fs;

  /* 'comp_filt_init:8' s.K_cf = K_cf; */
  s->K_cf = K_cf;

  /* 'comp_filt_init:9' s.dJ_max = dJ_max; */
  s->dJ_max = dJ_max;

  /* 'comp_filt_init:10' s.N_max = N_max; */
  s->N_max = N_max;

  /* 'comp_filt_init:11' s.alpha = alpha; */
  s->alpha = alpha;

  /* 'comp_filt_init:13' s.dt = 1 / fs; */
  s->dt = 1.0F / fs;

  /* 'comp_filt_init:15' qib_prev = [1 1 1 1]'; */
  /* 'comp_filt_init:16' s.qib_prev = qib_prev / norm(qib_prev); */
  for (i = 0; i < 4; i++) {
    s->qib_prev[i] = 0.5F;
  }
}

/*
 * File trailer for comp_filt_init.c
 *
 * [EOF]
 */
