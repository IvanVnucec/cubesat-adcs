/*
 * File: get_quat_from_K.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Jul-2021 11:13:20
 */

/* Include Files */
#include "get_quat_from_K.h"

#include "get_quat_from_K_data.h"
#include "optimal_request.h"
#include "optimal_request_init.h"
#include "vecnorm.h"

#include <math.h>

/* Function Definitions */

/*
 * function [q] = get_quat_from_K(K)
 * GET_QUAT_FROM_K Function returns the eigenvector of the matrix 'K' with the
 *    largest eigenvalue.
 *    The returned vector is quaternion rotation.
 *
 *    ===== References ======
 *    - Three-Axis Attitude Determination from Vector Observations,
 *      M. D. Shuster* and S.D.  OhtComputer Sciences Corporation,
 *      Silver Spring,  Md.
 *      J. GUIDANCE  AND CONTROL
 *
 *    ===== Changelog =====
 *    - July, 2021 - Replaced Matlab eig() function for finding eigenvalues
 *    of matrix K with different method used in the Reference above. This
 *    change might improove execution speed and also improve code generation
 *    with the Matlab Coder.
 *
 *  Author:     Ivan Vnucec
 *  University: FER, Zagreb
 *  Date:       May, 2021
 *  License:    MIT
 * Arguments    : const float K[16]
 *                float q[4]
 * Return Type  : void
 */
void get_quat_from_K(const float K[16], float q[4])
{
    int i0;
    int iy;
    float S[9];
    int jy;
    float Sigma;
    float Kappa;
    signed char ipiv[3];
    float x[9];
    int j;
    bool isodd;
    int b_tmp;
    int jp1j;
    int n;
    float y;
    int ix;
    float smax;
    float a_tmp;
    float s;
    float a;
    float b;
    float c;
    float b_K[3];
    int i1;
    int ijA;
    float Lambda;
    float X[3];

    /* 'get_quat_from_K:23' [~, S, z, ~] = get_util_matrices(K); */
    /* GET_UTIL_MATRICES Returns submatrices 'B', 'S', 'z' and 'Sigma' of 'K'  */
    /*    matrice. */
    /*    The submatrices are used for the calculation of Q matrice. */
    /*  */
    /*    Ref. B eq. 6 and 7, (for references list see main.m file under reference  */
    /*    comment section). */
    /*  =========================== Licence ============================ */
    /*  Author:     Ivan Vnucec */
    /*  University: FER, Zagreb */
    /*  Date:       May, 2021 */
    /*  License:    MIT */
    /* 'get_util_matrices:15' Sigma = K(4,4); */
    /* 'get_util_matrices:16' S = K(1:3,1:3) + Sigma * eye(3); */
    for (i0 = 0; i0 < 3; i0++) {
        iy        = i0 << 2;
        S[3 * i0] = K[iy] + K[15] * (float)iv0[3 * i0];
        jy        = 1 + 3 * i0;
        S[jy]     = K[1 + iy] + K[15] * (float)iv0[jy];
        jy        = 2 + 3 * i0;
        S[jy]     = K[2 + iy] + K[15] * (float)iv0[jy];
    }

    /* 'get_util_matrices:17' B = 0.5 * S; */
    /* 'get_util_matrices:18' z = K(1:3,4); */
    /* 'get_quat_from_K:23' ~ */
    /* 'get_quat_from_K:23' ~ */
    /* 'get_quat_from_K:25' s1_1 = S(1,1); */
    /* 'get_quat_from_K:26' s2_2 = S(2,2); */
    /* 'get_quat_from_K:27' s1_2 = S(1,2); */
    /* 'get_quat_from_K:28' s2_1 = S(2,1); */
    /* 'get_quat_from_K:29' s3_3 = S(3,3); */
    /* 'get_quat_from_K:30' s1_3 = S(1,3); */
    /* 'get_quat_from_K:31' s3_1 = S(3,1); */
    /* 'get_quat_from_K:32' s2_3 = S(2,3); */
    /* 'get_quat_from_K:33' s3_2 = S(3,2); */
    /* 'get_quat_from_K:35' Sigma = 1/2 * trace(S); */
    Sigma = 0.5F * ((S[0] + S[4]) + S[8]);

    /*  Kappa = trace(adjoint(S)), do explicitly because GNU Octave doesn't have */
    /*  ajdoint function yet */
    /* 'get_quat_from_K:38' Kappa = s1_1*s2_2 - s1_2*s2_1 + s1_1*s3_3 - s1_3*s3_1 + s2_2*s3_3 - s2_3*s3_2; */
    Kappa = ((((S[0] * S[4] - S[3] * S[1]) + S[0] * S[8]) - S[6] * S[2]) + S[4] * S[8])
            - S[7] * S[5];

    /* 'get_quat_from_K:39' Delta = det(S); */
    for (i0 = 0; i0 < 9; i0++) {
        x[i0] = S[i0];
    }

    ipiv[0] = 1;
    ipiv[1] = 2;
    for (j = 0; j < 2; j++) {
        b_tmp = j << 2;
        jp1j  = b_tmp + 2;
        n     = 3 - j;
        jy    = 0;
        ix    = b_tmp;
        smax  = fabsf(x[b_tmp]);
        for (iy = 2; iy <= n; iy++) {
            ix++;
            s = fabsf(x[ix]);
            if (s > smax) {
                jy   = iy - 1;
                smax = s;
            }
        }

        if (x[b_tmp + jy] != 0.0F) {
            if (jy != 0) {
                iy      = j + jy;
                ipiv[j] = (signed char)(iy + 1);
                smax    = x[j];
                x[j]    = x[iy];
                x[iy]   = smax;
                ix      = j + 3;
                iy += 3;
                smax  = x[ix];
                x[ix] = x[iy];
                x[iy] = smax;
                ix += 3;
                iy += 3;
                smax  = x[ix];
                x[ix] = x[iy];
                x[iy] = smax;
            }

            i0 = (b_tmp - j) + 3;
            for (jy = jp1j; jy <= i0; jy++) {
                x[jy - 1] /= x[b_tmp];
            }
        }

        n  = 1 - j;
        jy = b_tmp + 3;
        iy = b_tmp;
        for (jp1j = 0; jp1j <= n; jp1j++) {
            smax = x[jy];
            if (x[jy] != 0.0F) {
                ix = b_tmp + 1;
                i0 = iy + 5;
                i1 = (iy - j) + 6;
                for (ijA = i0; ijA <= i1; ijA++) {
                    x[ijA - 1] += x[ix] * -smax;
                    ix++;
                }
            }

            jy += 3;
            iy += 3;
        }
    }

    isodd = false;
    if (ipiv[0] > 1) {
        isodd = true;
    }

    y = x[0] * x[4] * x[8];
    if (ipiv[1] > 2) {
        isodd = !isodd;
    }

    if (isodd) {
        y = -y;
    }

    /* 'get_quat_from_K:41' a = Sigma^2 - Kappa; */
    a_tmp = Sigma * Sigma;
    a     = a_tmp - Kappa;

    /* 'get_quat_from_K:42' b = Sigma^2 + z' * z; */
    b = a_tmp + ((K[12] * K[12] + K[13] * K[13]) + K[14] * K[14]);

    /* 'get_quat_from_K:43' c = Delta + z' * S * z; */
    smax = 0.0F;
    for (i0 = 0; i0 < 3; i0++) {
        b_K[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
            iy    = i0 + 3 * i1;
            x[iy] = 0.0F;
            x[iy] = (S[i0] * S[3 * i1] + S[i0 + 3] * S[1 + 3 * i1])
                    + S[i0 + 6] * S[2 + 3 * i1];
            b_K[i0] += K[12 + i1] * S[i1 + 3 * i0];
        }

        smax += b_K[i0] * K[12 + i0];
    }

    c = y + smax;

    /* 'get_quat_from_K:44' d = z' * S^2 * z; */
    smax = 0.0F;
    for (i0 = 0; i0 < 3; i0++) {
        smax += ((K[12] * x[3 * i0] + K[13] * x[1 + 3 * i0]) + K[14] * x[2 + 3 * i0])
                * K[12 + i0];
    }

    /*  TODO: Check method in eq. 72 in the reference above where we have */
    /*  explicit equation for Lambda_max and we don't need to estimate it with */
    /*  the Newton-Raphson method */
    /*  Newton-Raphon method, 2 pass is enough */
    /*  f(x)  = x^4 - (a + b)*x^2 - c*x + (a*b + c*Sigma - d) */
    /*  f'(x) = 4*x^3 - 2*(a + b)*x - c */
    /* 'get_quat_from_K:53' Lambda = 1.0; */
    /*  initial guess */
    /* 'get_quat_from_K:54' for i = 1:2 */
    /*  Horner nested polynomial representation */
    /* 'get_quat_from_K:56' f = Sigma*c - d + a*b - Lambda*(c + Lambda*(- Lambda^2 + a + b)); */
    /* 'get_quat_from_K:57' df = - c - Lambda*(- 4*Lambda^2 + 2*a + 2*b); */
    /* 'get_quat_from_K:58' Lambda = Lambda - f/df; */
    smax = (Sigma * c - smax) + a * b;
    Lambda =
        1.0F - (smax - (c + ((-1.0F + a) + b))) / (-c - ((-4.0F + 2.0F * a) + 2.0F * b));

    /*  Horner nested polynomial representation */
    /* 'get_quat_from_K:56' f = Sigma*c - d + a*b - Lambda*(c + Lambda*(- Lambda^2 + a + b)); */
    /* 'get_quat_from_K:57' df = - c - Lambda*(- 4*Lambda^2 + 2*a + 2*b); */
    /* 'get_quat_from_K:58' Lambda = Lambda - f/df; */
    s = Lambda * Lambda;
    Lambda -= (smax - Lambda * (c + Lambda * ((-s + a) + b)))
              / (-c - Lambda * ((-4.0F * s + 2.0F * a) + 2.0F * b));

    /* 'get_quat_from_K:61' Alpha = Lambda^2 - Sigma^2 + Kappa; */
    s = (Lambda * Lambda - a_tmp) + Kappa;

    /* 'get_quat_from_K:62' Beta  = Lambda - Sigma; */
    a = Lambda - Sigma;

    /* 'get_quat_from_K:63' Gamma = (Lambda + Sigma) * Alpha - Delta; */
    smax = (Lambda + Sigma) * s - y;

    /* 'get_quat_from_K:65' X = (Alpha * eye(3) + Beta * S + S^2) * z; */
    for (i0 = 0; i0 < 9; i0++) {
        S[i0] = (s * (float)iv0[i0] + a * S[i0]) + x[i0];
    }

    for (i0 = 0; i0 < 3; i0++) {
        X[i0] = 0.0F;
        X[i0] = (S[i0] * K[12] + S[i0 + 3] * K[13]) + S[i0 + 6] * K[14];
    }

    /* 'get_quat_from_K:67' s = 1 / sqrt(Gamma^2 + vecnorm(X)^2); */
    a = vecnorm(X);
    a = 1.0F / sqrtf(smax * smax + a * a);

    /* 'get_quat_from_K:68' q = s * [X; Gamma]; */
    /*  because of Euler angles computation where we assume different quaternion */
    /*  representation */
    /* 'get_quat_from_K:72' q = [q(4); q(1:3)]; */
    q[0] = a * smax;
    q[1] = a * X[0];
    q[2] = a * X[1];
    q[3] = a * X[2];
}

/*
 * File trailer for get_quat_from_K.c
 *
 * [EOF]
 */
