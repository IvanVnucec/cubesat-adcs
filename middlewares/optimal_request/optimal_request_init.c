/*
 * File: optimal_request_init.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 10-Jul-2021 11:13:20
 */

/* Include Files */
#include "optimal_request_init.h"

#include "get_quat_from_K.h"
#include "get_quat_from_K_data.h"
#include "optimal_request.h"

/* Function Definitions */

/*
 * function [s] = optimal_request_init(s)
 * OPTIMAL_REQUEST_INIT Initialize the Optimal-REQUEST algorithm.
 *    Using first measurements r0 and b0, function sets the initial K, P and
 *    mk values used in the first iteration of the algorithm.
 *
 *  Ref B:
 *    Optimal-REQUEST Algorithm for Attitude Determination,
 *    D. Choukroun,I. Y. Bar-Itzhack, and Y. Oshman,
 *    https://sci-hub.se/10.2514/1.10337
 *
 *
 *  Handle structure:
 *  s = struct('w', zeros(3, 1), ...              % Angular velocity vector
 *      'r', r0, ...                              % reference sensor measurement
 *      'b', b0, ...                              % body sensor measurement
 *      'Mu_noise_var', meas.Mu_noise_var, ...    % sensor noise
 *      'Eta_noise_var', meas.Eta_noise_var, ...  % process noise
 *      'dT', meas.dT, ...                        % algorithm period of execution
 *      'K', zeros(4), ...                        % K matrix in Optimal-REQUEST algorithm
 *      'P', zeros(4), ...                        % P matrix...
 *      'mk', 0.0, ...                            % mk scalar...
 *      'Rho', 0.0);                              % Rho (Kalman gain)
 *
 *
 *  Author:     Ivan Vnucec
 *  University: FER, Zagreb
 *  Date:       May, 2021
 *  License:    MIT
 * Arguments    : struct0_T *s
 * Return Type  : void
 */
void optimal_request_init(struct0_T *s)
{
    int i5;
    float dB[9];
    float dz[3];
    float dSigma;
    int i;
    int rx_tmp_tmp;
    int dB_tmp_tmp;
    float fv1[3];
    float rx_tmp;
    float b_rx_tmp;
    float dB_tmp;
    float rx[9];
    float a;
    float b_s;
    float c_s[9];
    float d_s[9];
    float e_s[9];
    float b_rx[9];

    /*  unpack the structure for easier reading */
    /* 'optimal_request_init:31' r0 = s.r; */
    /* 'optimal_request_init:32' b0 = s.b; */
    /* 'optimal_request_init:33' Mu_noise_var = s.Mu_noise_var; */
    /* 'optimal_request_init:35' [~, ncols] = size(b0); */
    /* 'optimal_request_init:35' ~ */
    /* 'optimal_request_init:36' a0 = ones(1, ncols) ./ ncols; */
    /*  equal weights */
    /* 'optimal_request_init:38' dm0 = sum(a0); */
    /*  Ref. A eq. 11a */
    /* 'optimal_request_init:39' dK0 = calculate_dK(r0, b0, a0); */
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
    for (i5 = 0; i5 < 9; i5++) {
        dB[i5] = 0.0F;
    }

    /* 'calculate_dK:20' dz = zeros(3, 1); */
    dz[0] = 0.0F;
    dz[1] = 0.0F;
    dz[2] = 0.0F;

    /* 'calculate_dK:21' dSigma = 0.0; */
    dSigma = 0.0F;

    /* 'calculate_dK:22' for i = 1:ncols */
    for (i = 0; i < 2; i++) {
        /* 'calculate_dK:23' ai = a(i); */
        /* 'calculate_dK:24' bi = b(:,i); */
        /* 'calculate_dK:25' ri = r(:,i); */
        /* 'calculate_dK:27' dB = dB + ai .* bi * ri'; */
        /* 'calculate_dK:28' dz = dz + ai .* cross(bi, ri); */
        i5         = 2 + 3 * i;
        rx_tmp_tmp = 1 + 3 * i;
        fv1[0]     = 0.5F * (s->b[rx_tmp_tmp] * s->r[i5] - s->b[i5] * s->r[rx_tmp_tmp]);
        rx_tmp     = s->b[3 * i];
        b_rx_tmp   = s->r[3 * i];
        fv1[1]     = 0.5F * (s->b[2 + 3 * i] * b_rx_tmp - rx_tmp * s->r[2 + 3 * i]);
        fv1[2]     = 0.5F * (rx_tmp * s->r[1 + 3 * i] - s->b[1 + 3 * i] * b_rx_tmp);

        /* 'calculate_dK:29' dSigma = dSigma + ai * bi' * ri; */
        rx_tmp = 0.0F;
        for (i5 = 0; i5 < 3; i5++) {
            dB_tmp_tmp = i5 + 3 * i;
            dB_tmp     = 0.5F * s->b[dB_tmp_tmp];
            dB[i5] += dB_tmp * b_rx_tmp;
            dB[i5 + 3] += dB_tmp * s->r[1 + 3 * i];
            dB[i5 + 6] += dB_tmp * s->r[2 + 3 * i];
            dz[i5] += fv1[i5];
            rx_tmp += dB_tmp * s->r[dB_tmp_tmp];
        }

        dSigma += rx_tmp;
    }

    /* 'calculate_dK:31' dS = dB + dB'; */
    /* 'calculate_dK:33' dK = [dS - dSigma * eye(3), dz; dz', dSigma]; */
    for (i5 = 0; i5 < 3; i5++) {
        rx_tmp_tmp       = i5 << 2;
        s->K[rx_tmp_tmp] = (dB[3 * i5] + dB[i5]) - dSigma * (float)iv0[3 * i5];
        dB_tmp_tmp       = 1 + 3 * i5;
        s->K[1 + rx_tmp_tmp] =
            (dB[dB_tmp_tmp] + dB[i5 + 3]) - dSigma * (float)iv0[dB_tmp_tmp];
        dB_tmp_tmp = 2 + 3 * i5;
        s->K[2 + rx_tmp_tmp] =
            (dB[dB_tmp_tmp] + dB[i5 + 6]) - dSigma * (float)iv0[dB_tmp_tmp];
        s->K[12 + i5]        = dz[i5];
        s->K[3 + rx_tmp_tmp] = dz[i5];
    }

    s->K[15] = dSigma;

    /* 'optimal_request_init:40' R0 = calculate_R(r0, b0, Mu_noise_var); */
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
    for (i5 = 0; i5 < 9; i5++) {
        dB[i5] = 0.0F;
    }

    /* 'calculate_R:18' for i = 1 : ncols */
    rx[0] = 0.0F;
    rx[4] = 0.0F;
    rx[8] = 0.0F;
    for (i = 0; i < 2; i++) {
        /* 'calculate_R:19' ri = r(:, i); */
        /* 'calculate_R:20' bi = b(:, i); */
        /* 'calculate_R:22' rx = [0, -ri(3), ri(2); ri(3), 0, -ri(1); -ri(2), ri(1), 0]; */
        dB_tmp_tmp = 2 + 3 * i;
        dSigma     = s->r[dB_tmp_tmp];
        rx[3]      = -dSigma;
        rx_tmp_tmp = 1 + 3 * i;
        rx_tmp     = s->r[rx_tmp_tmp];
        rx[6]      = rx_tmp;
        rx[1]      = dSigma;
        b_rx_tmp   = s->r[3 * i];
        rx[7]      = -b_rx_tmp;
        rx[2]      = -rx_tmp;
        rx[5]      = b_rx_tmp;

        /* 'calculate_R:24' R11 = R11 + (3.0 - (ri' * bi)^2) * eye(3) + (bi' * ri) * (bi * ri' + ri * bi') ... */
        /* 'calculate_R:25'         + rx * (bi * bi') * rx'; */
        dB_tmp = s->b[3 * i];
        b_s = (b_rx_tmp * dB_tmp + rx_tmp * s->b[rx_tmp_tmp]) + dSigma * s->b[dB_tmp_tmp];
        a   = 3.0F - b_s * b_s;
        b_s = (dB_tmp * b_rx_tmp + s->b[1 + 3 * i] * rx_tmp) + s->b[2 + 3 * i] * dSigma;
        for (i5 = 0; i5 < 3; i5++) {
            dB_tmp_tmp  = i5 + 3 * i;
            c_s[i5]     = s->b[dB_tmp_tmp] * b_rx_tmp;
            d_s[i5]     = s->r[dB_tmp_tmp] * dB_tmp;
            e_s[i5]     = s->b[i5 + 3 * i] * dB_tmp;
            c_s[i5 + 3] = s->b[i5 + 3 * i] * rx_tmp;
            d_s[i5 + 3] = s->r[i5 + 3 * i] * s->b[1 + 3 * i];
            e_s[i5 + 3] = s->b[i5 + 3 * i] * s->b[1 + 3 * i];
            c_s[i5 + 6] = s->b[i5 + 3 * i] * dSigma;
            d_s[i5 + 6] = s->r[i5 + 3 * i] * s->b[2 + 3 * i];
            e_s[i5 + 6] = s->b[i5 + 3 * i] * s->b[2 + 3 * i];
        }

        for (i5 = 0; i5 < 3; i5++) {
            for (rx_tmp_tmp = 0; rx_tmp_tmp < 3; rx_tmp_tmp++) {
                dB_tmp_tmp       = i5 + 3 * rx_tmp_tmp;
                b_rx[dB_tmp_tmp] = 0.0F;
                b_rx[dB_tmp_tmp] =
                    (rx[i5] * e_s[3 * rx_tmp_tmp] + rx[i5 + 3] * e_s[1 + 3 * rx_tmp_tmp])
                    + rx[i5 + 6] * e_s[2 + 3 * rx_tmp_tmp];
            }

            for (rx_tmp_tmp = 0; rx_tmp_tmp < 3; rx_tmp_tmp++) {
                dB_tmp_tmp = i5 + 3 * rx_tmp_tmp;
                dB[dB_tmp_tmp] =
                    ((dB[dB_tmp_tmp] + a * (float)iv0[dB_tmp_tmp])
                     + b_s * (c_s[dB_tmp_tmp] + d_s[dB_tmp_tmp]))
                    + ((b_rx[i5] * rx[rx_tmp_tmp] + b_rx[i5 + 3] * rx[rx_tmp_tmp + 3])
                       + b_rx[i5 + 6] * rx[rx_tmp_tmp + 6]);
            }
        }
    }

    /* 'calculate_R:28' R11 = var / ncols * R11; */
    a = s->Mu_noise_var / 2.0F;
    for (i5 = 0; i5 < 9; i5++) {
        dB[i5] *= a;
    }

    /* 'calculate_R:29' R12 = zeros(3, 1); */
    /* 'calculate_R:30' R21 = R12'; */
    /* 'calculate_R:31' R22 = 2.0 * var / ncols; */
    /* 'calculate_R:33' R = [R11, R12; R21, R22]; */
    for (i5 = 0; i5 < 3; i5++) {
        rx_tmp_tmp           = i5 << 2;
        s->P[rx_tmp_tmp]     = dB[3 * i5];
        s->P[1 + rx_tmp_tmp] = dB[1 + 3 * i5];
        s->P[2 + rx_tmp_tmp] = dB[2 + 3 * i5];
        s->P[12 + i5]        = 0.0F;
        s->P[3 + rx_tmp_tmp] = 0.0F;
    }

    s->P[15] = 2.0F * s->Mu_noise_var / 2.0F;

    /* 'optimal_request_init:42' s.K = dK0; */
    /*  Ref. B eq. 65 */
    /* 'optimal_request_init:43' s.P = R0; */
    /*  Ref. B eq. 66 */
    /* 'optimal_request_init:44' s.mk = dm0; */
    s->mk = 1.0F;

    /*  Ref. B eq. 67, mk = m_k */
}

/*
 * File trailer for optimal_request_init.c
 *
 * [EOF]
 */
