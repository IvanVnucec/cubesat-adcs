/*
 * File: get_quat_from_K_types.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Jul-2021 11:13:20
 */

#ifndef GET_QUAT_FROM_K_TYPES_H
    #define GET_QUAT_FROM_K_TYPES_H

    /* Include Files */
    #include "rtwtypes.h"

    /* Type Definitions */
    #ifndef typedef_struct0_T
        #define typedef_struct0_T

typedef struct {
    float w[3];
    float r[6];
    float b[6];
    float Mu_noise_var;
    float Eta_noise_var;
    float dT;
    float K[16];
    float P[16];
    float mk;
    float Rho;
} struct0_T;

    #endif /*typedef_struct0_T*/
#endif

/*
 * File trailer for get_quat_from_K_types.h
 *
 * [EOF]
 */
