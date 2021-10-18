/*
 * File: optimal_request.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Jul-2021 11:13:20
 */

/* Include Files */
#include "optimal_request.h"

#include "get_quat_from_K.h"
#include "get_quat_from_K_data.h"
#include "optimal_request_init.h"
#include "vecnorm.h"

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Definitions */

/*
 * function [s] = optimal_request(s)
 * unpack structure for easier reading
 * Arguments    : struct0_T *s
 * Return Type  : void
 */
void optimal_request(struct0_T *s)
{
    float Theta;
    float a;
    float b_a;
    float Phi_tmp;
    float Phi[16];
    float b_Phi_tmp;
    float c_Phi_tmp;
    float dSigma;
    float Rho;
    float d_Phi_tmp;
    int i3;
    static const signed char b[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    int B_tmp;
    float B[9];
    int b_B_tmp;
    float b_B[9];
    int i4;
    float M[9];
    float yx[9];
    float rx[9];
    float Q12[3];
    float b_rx[3];
    float K_tmp[16];
    float b_Phi[16];
    float P[16];
    int i;
    float R[16];
    float m;
    float c_rx[9];

    /*  =========================== Notes ============================= */
    /*  Calculate one step of Optimal-REQUEST algorithm. */
    /*   */
    /*  Almost all of the algorithm equations have references to some */
    /*  paper. The reference equations are written in the form for ex. */
    /*  Ref A eq. 25 where letter 'A' denotes reference to the paper */
    /*  and the number '25' the equation number it that paper. The  */
    /*  references are listed under Reference section below. */
    /*   */
    /*  Indexes with k+1 are written without indexes and indexes with  */
    /*  k are written with _k. For example dm_k+1 is written as dm and */
    /*  dm_k is written as dmk. */
    /*  */
    /*  ========================= References ========================== */
    /*  Ref A:  */
    /*    REQUEST: A Recursive QUEST Algorithmfor Sequential Attitude Determination */
    /*    Itzhack Y. Bar-Itzhack,  */
    /*    https://sci-hub.se/https://doi.org/10.2514/3.21742 */
    /*  */
    /*  Ref B:  */
    /*    Optimal-REQUEST Algorithm for Attitude Determination, */
    /*    D. Choukroun,I. Y. Bar-Itzhack, and Y. Oshman, */
    /*    https://sci-hub.se/10.2514/1.10337 */
    /*  */
    /*  Ref. C:  */
    /*    Appendix B, Novel Methods for Attitude Determination Using Vector Observations, */
    /*    Daniel Choukroun, */
    /*    Page 233 */
    /*    https://www.researchgate.net/profile/Daniel-Choukroun/publication/265455600_Novel_Methods_for_Attitude_Determination_Using_Vector_Observations/links/5509c02b0cf26198a639a83c/Novel-Methods-for-Attitude-Determination-Using-Vector-Observations.pdf#page=253 */
    /*  */
    /*  Ref. D:  */
    /*    Appendixes, Attitude Determination Using Vector Observations and */
    /*    the Singular Value Decomposition */
    /*    Markley, F. L., */
    /*    http://malcolmdshuster.com/FC_Markley_1988_J_SVD_JAS_MDSscan.pdf */
    /*  */
    /*   */
    /*  Handle structure: */
    /*  s = struct('w', zeros(3, 1), ...              % Angular velocity vector */
    /*      'r', r0, ...                              % reference sensor measurement */
    /*      'b', b0, ...                              % body sensor measurement */
    /*      'Mu_noise_var', meas.Mu_noise_var, ...    % sensor noise */
    /*      'Eta_noise_var', meas.Eta_noise_var, ...  % process noise */
    /*      'dT', meas.dT, ...                        % algorithm period of execution */
    /*      'K', zeros(4), ...                        % K matrix in Optimal-REQUEST algorithm */
    /*      'P', zeros(4), ...                        % P matrix... */
    /*      'mk', 0.0, ...                            % mk scalar... */
    /*      'Rho', 0.0);                              % Rho (Kalman gain) */
    /*  */
    /*  */
    /*  =========================== Licence ============================ */
    /*  Author:     Ivan Vnucec */
    /*  University: FER, Zagreb */
    /*  Date:       May, 2021 */
    /*  License:    MIT */
    /* 'optimal_request:60' K = s.K; */
    /* 'optimal_request:61' P = s.P; */
    /* 'optimal_request:62' mk = s.mk; */
    /* 'optimal_request:63' w = s.w; */
    /* 'optimal_request:64' r = s.r; */
    /* 'optimal_request:65' b = s.b; */
    /* 'optimal_request:66' Mu_noise_var = s.Mu_noise_var; */
    /* 'optimal_request:67' Eta_noise_var = s.Eta_noise_var; */
    /* 'optimal_request:68' dT = s.dT; */
    /*  Calculate one step of Optimal-REQUEST algorithm. */
    /*  =================== time update =================== */
    /* 'optimal_request:74' Phi = calculate_Phi(w, dT); */
    /* CALCULATE_PHI Calculate matrix Phi (see Ref B eq. 9) matrix used by the  */
    /*  Optimal-REQUEST algorithm in the time update step. */
    /*   */
    /*  =========================== About ============================= */
    /*  The conventional method for computing the matrix exponential is to use */
    /*  MATLAB formula expm(). But as we calculate the matrix exponential of */
    /*  a 4x4 skew-symetric matrix in optimal_request.m file there exist */
    /*  simplification in paper below. */
    /*  The mathematic identities written below are devised by using the */
    /*  script in the misc/get_expm_skew_sym.m. User is advised to look into it. */
    /*  */
    /*  ========================= References ========================== */
    /*  Ref B:  */
    /*    Optimal-REQUEST Algorithm for Attitude Determination, */
    /*    D. Choukroun,I. Y. Bar-Itzhack, and Y. Oshman, */
    /*    https://sci-hub.se/10.2514/1.10337 */
    /*  */
    /*  Matrix exponential:  */
    /*  EXPONENTIALS OF REAL SKEW-SYMMETRIC MATRICES  IN TERMS OF THEIR EIGENVALUES  */
    /*  by Diego Gerardo Andree Avalos Galvez , 2018 */
    /*  see heading Case 4.4.1 */
    /*  */
    /*  =========================== Licence ============================ */
    /*  Author:     Ivan Vnucec */
    /*  University: FER, Zagreb */
    /*  Date:       Jul, 2021 */
    /*  License:    MIT */
    /*  Ref. B eq. 4 */
    /* 'calculate_Phi:31' wx = [0, -w(3), w(2); w(3), 0, -w(1); -w(2), w(1), 0]; */
    /*  Ref. B eq. 10 */
    /* 'calculate_Phi:34' Omega = 1.0 / 2 * [-wx, w; -w', 0]; */
    /*  calculate Phi */
    /*  Ref B eq. 9 with speed improvment over expm() function */
    /* 'calculate_Phi:38' S = Omega * dT; */
    /* 'calculate_Phi:39' Theta = dT / 2.0 * vecnorm(w); */
    Theta = s->dT / 2.0F * vecnorm(s->w);

    /* 'calculate_Phi:40' Phi = cos(Theta) * eye(4) + sin(Theta) / Theta * S; */
    a         = cosf(Theta);
    b_a       = sinf(Theta) / Theta;
    Phi_tmp   = b_a * (-0.0F * s->dT);
    Phi[0]    = Phi_tmp;
    Theta     = b_a * (0.5F * s->w[2] * s->dT);
    Phi[4]    = Theta;
    b_Phi_tmp = b_a * (0.5F * -s->w[1] * s->dT);
    Phi[8]    = b_Phi_tmp;
    c_Phi_tmp = b_a * (0.5F * -s->w[2] * s->dT);
    Phi[1]    = c_Phi_tmp;
    Phi[5]    = Phi_tmp;
    dSigma    = b_a * (0.5F * s->w[0] * s->dT);
    Phi[9]    = dSigma;
    Rho       = b_a * (0.5F * s->w[1] * s->dT);
    Phi[2]    = Rho;
    d_Phi_tmp = b_a * (0.5F * -s->w[0] * s->dT);
    Phi[6]    = d_Phi_tmp;
    Phi[10]   = Phi_tmp;
    Phi[12]   = dSigma;
    Phi[3]    = d_Phi_tmp;
    Phi[13]   = Rho;
    Phi[7]    = b_Phi_tmp;
    Phi[14]   = Theta;
    Phi[11]   = c_Phi_tmp;
    Phi[15]   = b_a * (0.0F * s->dT);
    for (i3 = 0; i3 < 16; i3++) {
        Phi[i3] += a * (float)b[i3];
    }

    /* 'optimal_request:76' [B, ~, z, Sigma] = get_util_matrices(K); */
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
    /* 'get_util_matrices:17' B = 0.5 * S; */
    Rho = s->K[15];
    for (i3 = 0; i3 < 3; i3++) {
        B_tmp      = i3 << 2;
        B[3 * i3]  = 0.5F * (s->K[B_tmp] + Rho * (float)iv0[3 * i3]);
        b_B_tmp    = 1 + 3 * i3;
        B[b_B_tmp] = 0.5F * (s->K[1 + B_tmp] + Rho * (float)iv0[b_B_tmp]);
        b_B_tmp    = 2 + 3 * i3;
        B[b_B_tmp] = 0.5F * (s->K[2 + B_tmp] + Rho * (float)iv0[b_B_tmp]);
    }

    /* 'get_util_matrices:18' z = K(1:3,4); */
    /* 'optimal_request:76' ~ */
    /* 'optimal_request:77' Q = calculate_Q(B, z, Sigma, Eta_noise_var, dT); */
    /* CALCULATE_Q Function computes the Q matrix used in Optimal-REQUEST  */
    /*             algorithm. */
    /*    Compute matrice 'Q' with submatrices 'B', 'z', 'Sigma', noise variance  */
    /*    variance 'var' and time difference 'dT'. 'B', 'z' and 'Sigma'  */
    /*    submatrices can be calculated via the 'get_util_matrices' function. */
    /*     */
    /*    Ref. C eq. B.2.1.4a - B.2.1.4d, (for references list see main.m file under reference  */
    /*    comment section). */
    /*  */
    /*  Author:     Ivan Vnucec */
    /*  University: FER, Zagreb */
    /*  Date:       May, 2021 */
    /*  License:    MIT */
    /* 'calculate_Q:16' M = B * (B - Sigma * eye(3)); */
    Rho = s->K[15];
    for (i3 = 0; i3 < 9; i3++) {
        b_B[i3] = B[i3] - Rho * (float)iv0[i3];
    }

    for (i3 = 0; i3 < 3; i3++) {
        for (i4 = 0; i4 < 3; i4++) {
            B_tmp    = i3 + 3 * i4;
            M[B_tmp] = 0.0F;
            M[B_tmp] = (B[i3] * b_B[3 * i4] + B[i3 + 3] * b_B[1 + 3 * i4])
                       + B[i3 + 6] * b_B[2 + 3 * i4];
        }
    }

    /* 'calculate_Q:17' yx = M' - M; */
    /* 'calculate_Q:18' y = [yx(3,2); yx(1,3); yx(2,1)]; */
    /* 'calculate_Q:20' Q11 = var * ((z' * z + Sigma^2 - trace(B * B')) * eye(3) ... */
    /* 'calculate_Q:21'     + 2 * (B' * B - B^2 - B'^2)); */
    for (i3 = 0; i3 < 3; i3++) {
        yx[3 * i3] = M[i3] - M[3 * i3];
        rx[3 * i3] = B[i3];
        B_tmp      = 1 + 3 * i3;
        yx[B_tmp]  = M[i3 + 3] - M[B_tmp];
        rx[B_tmp]  = B[i3 + 3];
        B_tmp      = 2 + 3 * i3;
        yx[B_tmp]  = M[i3 + 6] - M[B_tmp];
        rx[B_tmp]  = B[i3 + 6];
    }

    for (i3 = 0; i3 < 3; i3++) {
        for (i4 = 0; i4 < 3; i4++) {
            B_tmp    = i3 + 3 * i4;
            M[B_tmp] = 0.0F;
            M[B_tmp] = (B[i3] * rx[3 * i4] + B[i3 + 3] * rx[1 + 3 * i4])
                       + B[i3 + 6] * rx[2 + 3 * i4];
        }
    }

    d_Phi_tmp = s->K[15] * s->K[15];
    a = (((s->K[12] * s->K[12] + s->K[13] * s->K[13]) + s->K[14] * s->K[14]) + d_Phi_tmp)
        - ((M[0] + M[4]) + M[8]);

    /* 'calculate_Q:22' Q12 = -var * (y + B' * z); */
    Rho    = -s->Eta_noise_var;
    Q12[0] = yx[5];
    Q12[1] = yx[6];
    Q12[2] = yx[1];

    /* 'calculate_Q:23' Q21 = Q12'; */
    /* 'calculate_Q:24' Q22 = var * (trace(B * B') + Sigma^2 + z' * z); */
    for (i3 = 0; i3 < 3; i3++) {
        b_rx[i3] = 0.0F;
        for (i4 = 0; i4 < 3; i4++) {
            B_tmp    = i3 + 3 * i4;
            M[B_tmp] = 0.0F;
            M[B_tmp] = (B[i3] * rx[3 * i4] + B[i3 + 3] * rx[1 + 3 * i4])
                       + B[i3 + 6] * rx[2 + 3 * i4];
            b_rx[i3] += rx[B_tmp] * s->K[12 + i4];
        }

        Q12[i3] = Rho * (Q12[i3] + b_rx[i3]);
    }

    /* 'calculate_Q:26' Q = [Q11, Q12; Q21, Q22] * dT^2; */
    c_Phi_tmp = s->dT * s->dT;

    /*  Ref. B eq. 11 */
    /* 'optimal_request:80' K = Phi * K * Phi'; */
    /*  Ref. B eq. 69 */
    /* 'optimal_request:83' P = Phi * P * Phi' + Q; */
    for (i3 = 0; i3 < 4; i3++) {
        for (i4 = 0; i4 < 4; i4++) {
            B_tmp                 = i4 << 2;
            b_B_tmp               = i3 + B_tmp;
            K_tmp[i4 + (i3 << 2)] = Phi[b_B_tmp];
            b_Phi[b_B_tmp]        = 0.0F;
            b_Phi[b_B_tmp] = ((Phi[i3] * s->P[B_tmp] + Phi[i3 + 4] * s->P[1 + B_tmp])
                              + Phi[i3 + 8] * s->P[2 + B_tmp])
                             + Phi[i3 + 12] * s->P[3 + B_tmp];
        }
    }

    Rho = 0.0F;
    for (i3 = 0; i3 < 3; i3++) {
        for (i4 = 0; i4 < 3; i4++) {
            b_a       = B[3 * i4];
            B_tmp     = 1 + 3 * i4;
            Theta     = rx[i3 + 3];
            b_B_tmp   = 2 + 3 * i4;
            b_Phi_tmp = rx[i3 + 6];
            yx[i3 + 3 * i4] =
                (((rx[i3] * b_a + Theta * B[B_tmp]) + b_Phi_tmp * B[b_B_tmp])
                 - ((B[i3] * b_a + B[i3 + 3] * B[B_tmp]) + B[i3 + 6] * B[b_B_tmp]))
                - ((rx[i3] * rx[3 * i4] + Theta * rx[B_tmp]) + b_Phi_tmp * rx[b_B_tmp]);
        }

        b_Phi_tmp = s->K[12 + i3];
        Rho += b_Phi_tmp * b_Phi_tmp;
    }

    for (i3 = 0; i3 < 4; i3++) {
        for (i4 = 0; i4 < 4; i4++) {
            B_tmp      = i4 << 2;
            b_B_tmp    = i3 + B_tmp;
            P[b_B_tmp] = 0.0F;
            P[b_B_tmp] = ((b_Phi[i3] * K_tmp[B_tmp] + b_Phi[i3 + 4] * K_tmp[1 + B_tmp])
                          + b_Phi[i3 + 8] * K_tmp[2 + B_tmp])
                         + b_Phi[i3 + 12] * K_tmp[3 + B_tmp];
        }
    }

    for (i3 = 0; i3 < 3; i3++) {
        B_tmp = i3 << 2;
        b_Phi[B_tmp] =
            s->Eta_noise_var * (a * (float)iv0[3 * i3] + 2.0F * yx[3 * i3]) * c_Phi_tmp;
        b_B_tmp = 1 + 3 * i3;
        b_Phi[1 + B_tmp] =
            s->Eta_noise_var * (a * (float)iv0[b_B_tmp] + 2.0F * yx[b_B_tmp]) * c_Phi_tmp;
        b_B_tmp = 2 + 3 * i3;
        b_Phi[2 + B_tmp] =
            s->Eta_noise_var * (a * (float)iv0[b_B_tmp] + 2.0F * yx[b_B_tmp]) * c_Phi_tmp;
        Phi_tmp          = Q12[i3] * c_Phi_tmp;
        b_Phi[12 + i3]   = Phi_tmp;
        b_Phi[3 + B_tmp] = Phi_tmp;
    }

    b_Phi[15] =
        s->Eta_noise_var * ((((M[0] + M[4]) + M[8]) + d_Phi_tmp) + Rho) * c_Phi_tmp;
    for (i3 = 0; i3 < 16; i3++) {
        P[i3] += b_Phi[i3];
    }

    /*  ================ measurement update =============== */
    /*  calc. meas. weights */
    /* 'optimal_request:87' [~, ncols] = size(b); */
    /* 'optimal_request:87' ~ */
    /* 'optimal_request:88' a = ones(1, ncols) ./ ncols; */
    /*  equal weights  */
    /* 'optimal_request:90' dm = sum(a); */
    /* 'optimal_request:92' R = calculate_R(r, b, Mu_noise_var); */
    /* CALCULATE_R Function computes the R matrix used in Optimal-REQUEST  */
    /*             algorithm. */
    /*    Compute matrice 'R' with reference matrice 'r', body measurements */
    /*    matrice 'b' and measurement variance 'var'. */
    /*  */
    /*    Ref. C eq. B.2.2.4a - B.2.2.4d, (for references list see main.m file under reference  */
    /*    comment section). */
    /*  */
    /*  Author:     Ivan Vnucec */
    /*  University: FER, Zagreb */
    /*  Date:       May, 2021 */
    /*  License:    MIT */
    /* 'calculate_R:15' [~, ncols] = size(r); */
    /* 'calculate_R:15' ~ */
    /* 'calculate_R:17' R11 = zeros(3, 3); */
    for (i3 = 0; i3 < 9; i3++) {
        M[i3] = 0.0F;
    }

    /* 'calculate_R:18' for i = 1 : ncols */
    rx[0] = 0.0F;
    rx[4] = 0.0F;
    rx[8] = 0.0F;
    for (i = 0; i < 2; i++) {
        /* 'calculate_R:19' ri = r(:, i); */
        /* 'calculate_R:20' bi = b(:, i); */
        /* 'calculate_R:22' rx = [0, -ri(3), ri(2); ri(3), 0, -ri(1); -ri(2), ri(1), 0]; */
        B_tmp     = 2 + 3 * i;
        c_Phi_tmp = s->r[B_tmp];
        rx[3]     = -c_Phi_tmp;
        b_B_tmp   = 1 + 3 * i;
        d_Phi_tmp = s->r[b_B_tmp];
        rx[6]     = d_Phi_tmp;
        rx[1]     = c_Phi_tmp;
        Theta     = s->r[3 * i];
        rx[7]     = -Theta;
        rx[2]     = -d_Phi_tmp;
        rx[5]     = Theta;

        /* 'calculate_R:24' R11 = R11 + (3.0 - (ri' * bi)^2) * eye(3) + (bi' * ri) * (bi * ri' + ri * bi') ... */
        /* 'calculate_R:25'         + rx * (bi * bi') * rx'; */
        b_Phi_tmp = s->b[3 * i];
        Rho = (Theta * b_Phi_tmp + d_Phi_tmp * s->b[b_B_tmp]) + c_Phi_tmp * s->b[B_tmp];
        a   = 3.0F - Rho * Rho;
        Rho = (b_Phi_tmp * Theta + s->b[1 + 3 * i] * d_Phi_tmp)
              + s->b[2 + 3 * i] * c_Phi_tmp;
        for (i3 = 0; i3 < 3; i3++) {
            B_tmp       = i3 + 3 * i;
            yx[i3]      = s->b[B_tmp] * Theta;
            B[i3]       = s->r[B_tmp] * b_Phi_tmp;
            b_B[i3]     = s->b[i3 + 3 * i] * b_Phi_tmp;
            yx[i3 + 3]  = s->b[i3 + 3 * i] * d_Phi_tmp;
            B[i3 + 3]   = s->r[i3 + 3 * i] * s->b[1 + 3 * i];
            b_B[i3 + 3] = s->b[i3 + 3 * i] * s->b[1 + 3 * i];
            yx[i3 + 6]  = s->b[i3 + 3 * i] * c_Phi_tmp;
            B[i3 + 6]   = s->r[i3 + 3 * i] * s->b[2 + 3 * i];
            b_B[i3 + 6] = s->b[i3 + 3 * i] * s->b[2 + 3 * i];
        }

        for (i3 = 0; i3 < 3; i3++) {
            for (i4 = 0; i4 < 3; i4++) {
                b_B_tmp       = i3 + 3 * i4;
                c_rx[b_B_tmp] = 0.0F;
                c_rx[b_B_tmp] = (rx[i3] * b_B[3 * i4] + rx[i3 + 3] * b_B[1 + 3 * i4])
                                + rx[i3 + 6] * b_B[2 + 3 * i4];
            }

            for (i4 = 0; i4 < 3; i4++) {
                B_tmp = i3 + 3 * i4;
                M[B_tmp] =
                    ((M[B_tmp] + a * (float)iv0[B_tmp]) + Rho * (yx[B_tmp] + B[B_tmp]))
                    + ((c_rx[i3] * rx[i4] + c_rx[i3 + 3] * rx[i4 + 3])
                       + c_rx[i3 + 6] * rx[i4 + 6]);
            }
        }
    }

    /* 'calculate_R:28' R11 = var / ncols * R11; */
    a = s->Mu_noise_var / 2.0F;
    for (i3 = 0; i3 < 9; i3++) {
        M[i3] *= a;
    }

    /* 'calculate_R:29' R12 = zeros(3, 1); */
    /* 'calculate_R:30' R21 = R12'; */
    /* 'calculate_R:31' R22 = 2.0 * var / ncols; */
    /* 'calculate_R:33' R = [R11, R12; R21, R22]; */
    for (i3 = 0; i3 < 3; i3++) {
        B_tmp        = i3 << 2;
        R[B_tmp]     = M[3 * i3];
        R[1 + B_tmp] = M[1 + 3 * i3];
        R[2 + B_tmp] = M[2 + 3 * i3];
        R[12 + i3]   = 0.0F;
        R[3 + B_tmp] = 0.0F;
    }

    R[15] = 2.0F * s->Mu_noise_var / 2.0F;

    /*  Ref. B eq. 70 */
    /* 'optimal_request:95' Rho = (mk^2 * trace(P)) / (mk^2 * trace(P) + dm^2 * trace(R)); */
    Theta = s->mk * s->mk;
    Rho =
        Theta * (((P[0] + P[5]) + P[10]) + P[15])
        / (Theta * (((P[0] + P[5]) + P[10]) + P[15]) + (((R[0] + R[5]) + R[10]) + R[15]));

    /*  Ref. B eq. 71 */
    /* 'optimal_request:98' m = (1.0 - Rho) * mk + Rho * dm; */
    b_Phi_tmp = (1.0F - Rho) * s->mk;
    m         = b_Phi_tmp + Rho;

    /* 'optimal_request:100' dK = calculate_dK(r, b, a); */
    /* CALCULATE_DK Function computes the incremental dK matrix used in  */
    /*              Optimal-REQUEST algorithm. */
    /*    Calculate 'dK' incremental matrice with reference matrice 'r', body  */
    /*    matrice 'b' and weights assigned to each column vector in body matrice  */
    /*    'a'. */
    /*  */
    /*    Ref. A eq. 11b - 11f, (for references list see main.m file under reference  */
    /*    comment section). */
    /*  */
    /*  Author:     Ivan Vnucec */
    /*  University: FER, Zagreb */
    /*  Date:       May, 2021 */
    /*  License:    MIT */
    /* 'calculate_dK:16' [~, ncols] = size(r); */
    /* 'calculate_dK:16' ~ */
    /*  eqs. 3.7 */
    /* 'calculate_dK:19' dB = zeros(3, 3); */
    for (i3 = 0; i3 < 9; i3++) {
        M[i3] = 0.0F;
    }

    /* 'calculate_dK:20' dz = zeros(3, 1); */
    Q12[0] = 0.0F;
    Q12[1] = 0.0F;
    Q12[2] = 0.0F;

    /* 'calculate_dK:21' dSigma = 0.0; */
    dSigma = 0.0F;

    /* 'calculate_dK:22' for i = 1:ncols */
    for (i = 0; i < 2; i++) {
        /* 'calculate_dK:23' ai = a(i); */
        /* 'calculate_dK:24' bi = b(:,i); */
        /* 'calculate_dK:25' ri = r(:,i); */
        /* 'calculate_dK:27' dB = dB + ai .* bi * ri'; */
        /* 'calculate_dK:28' dz = dz + ai .* cross(bi, ri); */
        b_B_tmp   = 2 + 3 * i;
        B_tmp     = 1 + 3 * i;
        b_rx[0]   = 0.5F * (s->b[B_tmp] * s->r[b_B_tmp] - s->b[b_B_tmp] * s->r[B_tmp]);
        c_Phi_tmp = s->b[3 * i];
        d_Phi_tmp = s->r[3 * i];
        b_rx[1]   = 0.5F * (s->b[2 + 3 * i] * d_Phi_tmp - c_Phi_tmp * s->r[2 + 3 * i]);
        b_rx[2]   = 0.5F * (c_Phi_tmp * s->r[1 + 3 * i] - s->b[1 + 3 * i] * d_Phi_tmp);

        /* 'calculate_dK:29' dSigma = dSigma + ai * bi' * ri; */
        b_a = 0.0F;
        for (i3 = 0; i3 < 3; i3++) {
            B_tmp = i3 + 3 * i;
            Theta = 0.5F * s->b[B_tmp];
            M[i3] += Theta * d_Phi_tmp;
            M[i3 + 3] += Theta * s->r[1 + 3 * i];
            M[i3 + 6] += Theta * s->r[2 + 3 * i];
            Q12[i3] += b_rx[i3];
            b_a += Theta * s->r[B_tmp];
        }

        dSigma += b_a;
    }

    /* 'calculate_dK:31' dS = dB + dB'; */
    /* 'calculate_dK:33' dK = [dS - dSigma * eye(3), dz; dz', dSigma]; */
    /*  Ref. B eq. 72 */
    /* 'optimal_request:103' s.K = (1.0 - Rho) * mk / m * K + Rho * dm / m * dK; */
    d_Phi_tmp = b_Phi_tmp / m;
    Theta     = Rho / m;
    for (i3 = 0; i3 < 4; i3++) {
        for (i4 = 0; i4 < 4; i4++) {
            B_tmp          = i4 << 2;
            b_B_tmp        = i3 + B_tmp;
            b_Phi[b_B_tmp] = 0.0F;
            b_Phi[b_B_tmp] = ((Phi[i3] * s->K[B_tmp] + Phi[i3 + 4] * s->K[1 + B_tmp])
                              + Phi[i3 + 8] * s->K[2 + B_tmp])
                             + Phi[i3 + 12] * s->K[3 + B_tmp];
        }
    }

    for (i3 = 0; i3 < 4; i3++) {
        for (i4 = 0; i4 < 4; i4++) {
            B_tmp        = i4 << 2;
            b_B_tmp      = i3 + B_tmp;
            Phi[b_B_tmp] = 0.0F;
            Phi[b_B_tmp] = ((b_Phi[i3] * K_tmp[B_tmp] + b_Phi[i3 + 4] * K_tmp[1 + B_tmp])
                            + b_Phi[i3 + 8] * K_tmp[2 + B_tmp])
                           + b_Phi[i3 + 12] * K_tmp[3 + B_tmp];
        }
    }

    for (i3 = 0; i3 < 3; i3++) {
        B_tmp        = i3 << 2;
        b_Phi[B_tmp] = Theta * ((M[3 * i3] + M[i3]) - dSigma * (float)iv0[3 * i3]);
        b_B_tmp      = 1 + 3 * i3;
        b_Phi[1 + B_tmp] =
            Theta * ((M[b_B_tmp] + M[i3 + 3]) - dSigma * (float)iv0[b_B_tmp]);
        b_B_tmp = 2 + 3 * i3;
        b_Phi[2 + B_tmp] =
            Theta * ((M[b_B_tmp] + M[i3 + 6]) - dSigma * (float)iv0[b_B_tmp]);
        Phi_tmp          = Theta * Q12[i3];
        b_Phi[12 + i3]   = Phi_tmp;
        b_Phi[3 + B_tmp] = Phi_tmp;
    }

    b_Phi[15] = Theta * dSigma;

    /*  Ref. B eq. 73 */
    /* 'optimal_request:106' s.P = ((1.0 - Rho) * mk / m)^2 * P + (Rho * dm / m)^2 * R; */
    a   = d_Phi_tmp * d_Phi_tmp;
    b_a = Theta * Theta;
    for (i3 = 0; i3 < 16; i3++) {
        s->K[i3] = d_Phi_tmp * Phi[i3] + b_Phi[i3];
        s->P[i3] = a * P[i3] + b_a * R[i3];
    }

    /*  for the next iteration m_k = m_k+1 */
    /* 'optimal_request:109' s.mk = m; */
    s->mk = m;

    /*  save Rho into the structure for debug purposes */
    /* 'optimal_request:112' s.Rho = Rho; */
    s->Rho = Rho;
}

#ifdef __cplusplus
}
#endif

/*
 * File trailer for optimal_request.c
 *
 * [EOF]
 */
