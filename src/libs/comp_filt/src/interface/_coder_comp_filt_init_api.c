/*
 * File: _coder_comp_filt_init_api.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 15-Jun-2022 15:26:49
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_comp_filt_init_api.h"
#include "_coder_comp_filt_init_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true, false, 131434U, NULL, "comp_filt_init",
  NULL, false, { 2045744189U, 2170104910U, 2743257031U, 4284093946U }, NULL };

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[3]);
static const mxArray *b_emlrt_marshallOut(const real32_T u[4]);
static real32_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *fs,
  const char_T *identifier);
static real32_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static struct0_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *s,
  const char_T *identifier);
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *acc_i, const
  char_T *identifier, real32_T y[3]);
static const mxArray *emlrt_marshallOut(const struct0_T u);
static struct0_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId);
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[4]);
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[3]);
static real32_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId);
static void j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[4]);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real32_T y[3]
 * Return Type  : void
 */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[3])
{
  h_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const real32_T u[4]
 * Return Type  : const mxArray *
 */
static const mxArray *b_emlrt_marshallOut(const real32_T u[4])
{
  const mxArray *y;
  const mxArray *m1;
  static const int32_T iv2[1] = { 4 };

  real32_T *pData;
  int32_T i;
  y = NULL;
  m1 = emlrtCreateNumericArray(1, iv2, mxSINGLE_CLASS, mxREAL);
  pData = (real32_T *)mxGetData(m1);
  for (i = 0; i < 4; i++) {
    pData[i] = u[i];
  }

  emlrtAssign(&y, m1);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *fs
 *                const char_T *identifier
 * Return Type  : real32_T
 */
static real32_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *fs,
  const char_T *identifier)
{
  real32_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(fs), &thisId);
  emlrtDestroyArray(&fs);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real32_T
 */
static real32_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real32_T y;
  y = i_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *s
 *                const char_T *identifier
 * Return Type  : struct0_T
 */
static struct0_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *s,
  const char_T *identifier)
{
  struct0_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = f_emlrt_marshallIn(sp, emlrtAlias(s), &thisId);
  emlrtDestroyArray(&s);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *acc_i
 *                const char_T *identifier
 *                real32_T y[3]
 * Return Type  : void
 */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *acc_i, const
  char_T *identifier, real32_T y[3])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(acc_i), &thisId, y);
  emlrtDestroyArray(&acc_i);
}

/*
 * Arguments    : const struct0_T u
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const struct0_T u)
{
  const mxArray *y;
  const mxArray *b_y;
  const mxArray *m0;
  static const int32_T iv0[1] = { 3 };

  real32_T *pData;
  int32_T i;
  const mxArray *c_y;
  static const int32_T iv1[1] = { 3 };

  const mxArray *d_y;
  const mxArray *e_y;
  const mxArray *f_y;
  const mxArray *g_y;
  const mxArray *h_y;
  const mxArray *i_y;
  y = NULL;
  emlrtAssign(&y, emlrtCreateStructMatrix(1, 1, 0, NULL));
  b_y = NULL;
  m0 = emlrtCreateNumericArray(1, iv0, mxSINGLE_CLASS, mxREAL);
  pData = (real32_T *)mxGetData(m0);
  for (i = 0; i < 3; i++) {
    pData[i] = u.acc_i[i];
  }

  emlrtAssign(&b_y, m0);
  emlrtAddField(y, b_y, "acc_i", 0);
  c_y = NULL;
  m0 = emlrtCreateNumericArray(1, iv1, mxSINGLE_CLASS, mxREAL);
  pData = (real32_T *)mxGetData(m0);
  for (i = 0; i < 3; i++) {
    pData[i] = u.mag_i[i];
  }

  emlrtAssign(&c_y, m0);
  emlrtAddField(y, c_y, "mag_i", 0);
  d_y = NULL;
  m0 = emlrtCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
  *(real32_T *)mxGetData(m0) = u.fs;
  emlrtAssign(&d_y, m0);
  emlrtAddField(y, d_y, "fs", 0);
  e_y = NULL;
  m0 = emlrtCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
  *(real32_T *)mxGetData(m0) = u.K_cf;
  emlrtAssign(&e_y, m0);
  emlrtAddField(y, e_y, "K_cf", 0);
  f_y = NULL;
  m0 = emlrtCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
  *(real32_T *)mxGetData(m0) = u.dJ_max;
  emlrtAssign(&f_y, m0);
  emlrtAddField(y, f_y, "dJ_max", 0);
  g_y = NULL;
  m0 = emlrtCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
  *(real32_T *)mxGetData(m0) = u.N_max;
  emlrtAssign(&g_y, m0);
  emlrtAddField(y, g_y, "N_max", 0);
  h_y = NULL;
  m0 = emlrtCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
  *(real32_T *)mxGetData(m0) = u.alpha;
  emlrtAssign(&h_y, m0);
  emlrtAddField(y, h_y, "alpha", 0);
  i_y = NULL;
  m0 = emlrtCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
  *(real32_T *)mxGetData(m0) = u.dt;
  emlrtAssign(&i_y, m0);
  emlrtAddField(y, i_y, "dt", 0);
  emlrtAddField(y, b_emlrt_marshallOut(u.qib_prev), "qib_prev", 0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : struct0_T
 */
static struct0_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId)
{
  struct0_T y;
  emlrtMsgIdentifier thisId;
  static const char * fieldNames[9] = { "acc_i", "mag_i", "fs", "K_cf", "dJ_max",
    "N_max", "alpha", "dt", "qib_prev" };

  static const int32_T dims = 0;
  thisId.fParent = parentId;
  thisId.bParentIsCell = false;
  emlrtCheckStructR2012b(sp, parentId, u, 9, fieldNames, 0U, &dims);
  thisId.fIdentifier = "acc_i";
  b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "acc_i")),
                     &thisId, y.acc_i);
  thisId.fIdentifier = "mag_i";
  b_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "mag_i")),
                     &thisId, y.mag_i);
  thisId.fIdentifier = "fs";
  y.fs = d_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "fs")),
    &thisId);
  thisId.fIdentifier = "K_cf";
  y.K_cf = d_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "K_cf")), &thisId);
  thisId.fIdentifier = "dJ_max";
  y.dJ_max = d_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "dJ_max")), &thisId);
  thisId.fIdentifier = "N_max";
  y.N_max = d_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "N_max")), &thisId);
  thisId.fIdentifier = "alpha";
  y.alpha = d_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0,
    "alpha")), &thisId);
  thisId.fIdentifier = "dt";
  y.dt = d_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "dt")),
    &thisId);
  thisId.fIdentifier = "qib_prev";
  g_emlrt_marshallIn(sp, emlrtAlias(emlrtGetFieldR2013a(sp, u, 0, "qib_prev")),
                     &thisId, y.qib_prev);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                real32_T y[4]
 * Return Type  : void
 */
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, real32_T y[4])
{
  j_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real32_T ret[3]
 * Return Type  : void
 */
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[3])
{
  static const int32_T dims[1] = { 3 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single|double", false, 1U, dims);
  emlrtImportArrayR2015b(sp, src, ret, 4, false);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real32_T
 */
static real32_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId)
{
  real32_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "single|double", false, 0U, &dims);
  emlrtImportArrayR2015b(sp, src, &ret, 4, false);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                real32_T ret[4]
 * Return Type  : void
 */
static void j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, real32_T ret[4])
{
  static const int32_T dims[1] = { 4 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "single|double", false, 1U, dims);
  emlrtImportArrayR2015b(sp, src, ret, 4, false);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const mxArray * const prhs[7]
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void comp_filt_init_api(const mxArray * const prhs[7], const mxArray *plhs[1])
{
  real32_T acc_i[3];
  real32_T mag_i[3];
  real32_T fs;
  real32_T K_cf;
  real32_T dJ_max;
  real32_T N_max;
  real32_T alpha;
  struct0_T s;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "acc_i", acc_i);
  emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "mag_i", mag_i);
  fs = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "fs");
  K_cf = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "K_cf");
  dJ_max = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "dJ_max");
  N_max = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[5]), "N_max");
  alpha = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "alpha");

  /* Invoke the target function */
  comp_filt_init(acc_i, mag_i, fs, K_cf, dJ_max, N_max, alpha, &s);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(s);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void comp_filt_init_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  comp_filt_init_xil_terminate();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void comp_filt_init_initialize(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void comp_filt_init_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * Arguments    : const mxArray * const prhs[4]
 *                const mxArray *plhs[2]
 * Return Type  : void
 */
void comp_filt_step_api(const mxArray * const prhs[4], const mxArray *plhs[2])
{
  struct0_T s;
  real32_T acc_b[3];
  real32_T mag_b[3];
  real32_T gyr_b[3];
  real32_T qib_est[4];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Marshall function inputs */
  s = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "s");
  emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "acc_b", acc_b);
  emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "mag_b", mag_b);
  emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "gyr_b", gyr_b);

  /* Invoke the target function */
  comp_filt_step(&s, acc_b, mag_b, gyr_b, qib_est);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(s);
  plhs[1] = b_emlrt_marshallOut(qib_est);
}

/*
 * File trailer for _coder_comp_filt_init_api.c
 *
 * [EOF]
 */
