/*
 * File: main.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 15-Jun-2022 15:26:49
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/
/* Include Files */
#include "rt_nonfinite.h"
#include "comp_filt_init.h"
#include "comp_filt_step.h"
#include "main.h"
#include "comp_filt_init_terminate.h"
#include "comp_filt_init_initialize.h"

/* Function Declarations */
static void argInit_3x1_real32_T(float result[3]);
static void argInit_4x1_real32_T(float result[4]);
static float argInit_real32_T(void);
static struct0_T argInit_struct0_T(void);
static void main_comp_filt_init(void);
static void main_comp_filt_step(void);

/* Function Definitions */

/*
 * Arguments    : float result[3]
 * Return Type  : void
 */
static void argInit_3x1_real32_T(float result[3])
{
  int idx0;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 3; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0] = argInit_real32_T();
  }
}

/*
 * Arguments    : float result[4]
 * Return Type  : void
 */
static void argInit_4x1_real32_T(float result[4])
{
  int idx0;

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 4; idx0++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[idx0] = argInit_real32_T();
  }
}

/*
 * Arguments    : void
 * Return Type  : float
 */
static float argInit_real32_T(void)
{
  return 0.0F;
}

/*
 * Arguments    : void
 * Return Type  : struct0_T
 */
static struct0_T argInit_struct0_T(void)
{
  struct0_T result;

  /* Set the value of each structure field.
     Change this value to the value that the application requires. */
  argInit_3x1_real32_T(result.acc_i);
  argInit_3x1_real32_T(result.mag_i);
  result.fs = argInit_real32_T();
  result.K_cf = argInit_real32_T();
  result.dJ_max = argInit_real32_T();
  result.N_max = argInit_real32_T();
  result.alpha = argInit_real32_T();
  result.dt = argInit_real32_T();
  argInit_4x1_real32_T(result.qib_prev);
  return result;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_comp_filt_init(void)
{
  float fv0[3];
  float fv1[3];
  struct0_T s;

  /* Initialize function 'comp_filt_init' input arguments. */
  /* Initialize function input argument 'acc_i'. */
  /* Initialize function input argument 'mag_i'. */
  /* Call the entry-point 'comp_filt_init'. */
  argInit_3x1_real32_T(fv0);
  argInit_3x1_real32_T(fv1);
  comp_filt_init(fv0, fv1, argInit_real32_T(), argInit_real32_T(),
                 argInit_real32_T(), argInit_real32_T(), argInit_real32_T(), &s);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_comp_filt_step(void)
{
  struct0_T s;
  float fv2[3];
  float fv3[3];
  float fv4[3];
  float qib_est[4];

  /* Initialize function 'comp_filt_step' input arguments. */
  /* Initialize function input argument 's'. */
  s = argInit_struct0_T();

  /* Initialize function input argument 'acc_b'. */
  /* Initialize function input argument 'mag_b'. */
  /* Initialize function input argument 'gyr_b'. */
  /* Call the entry-point 'comp_filt_step'. */
  argInit_3x1_real32_T(fv2);
  argInit_3x1_real32_T(fv3);
  argInit_3x1_real32_T(fv4);
  comp_filt_step(&s, fv2, fv3, fv4, qib_est);
}

/*
 * Arguments    : int argc
 *                const char * const argv[]
 * Return Type  : int
 */
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* Initialize the application.
     You do not need to do this more than one time. */
  comp_filt_init_initialize();

  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_comp_filt_init();
  main_comp_filt_step();

  /* Terminate the application.
     You do not need to do this more than one time. */
  comp_filt_init_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
