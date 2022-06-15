/*
 * File: comp_filt_step.c
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 15-Jun-2022 15:26:49
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "comp_filt_init.h"
#include "comp_filt_step.h"
#include "norm.h"

/* Function Definitions */

/*
 * COMP_FILT_STEP Summary of this function goes here
 *    Detailed explanation goes here
 * Arguments    : struct0_T *s
 *                float acc_b[3]
 *                float mag_b[3]
 *                const float gyr_b[3]
 *                float qib_est[4]
 * Return Type  : void
 */
void comp_filt_step(struct0_T *s, float acc_b[3], float mag_b[3], const float
                    gyr_b[3], float qib_est[4])
{
  float value;
  float b_value;
  int i0;
  float fv5[16];
  float fv6[16];
  float fv7[4];
  int i1;
  float qib_gd[4];
  int i;
  float b_qib_est;
  float c_value;
  float d_value;
  float a_i[3];
  float m_i[3];
  float a_b[3];
  float m_b[3];
  float J_previous;
  boolean_T exitg1;
  float Rib[9];
  float e_a[3];
  float e_m[3];
  float J_current;
  float b_qib_gd[9];
  float c_qib_gd[9];
  float d_qib_gd[9];
  float e_qib_gd[9];
  float fv8[3];
  float fv9[3];
  float M2[9];
  float M3[9];
  float M4[9];
  float grad_J[4];

  /*  Normalize measurements */
  /* 'comp_filt_step:6' acc_b = acc_b / norm(acc_b); */
  value = norm(acc_b);

  /*  normalized column vector in NED */
  /* 'comp_filt_step:7' mag_b = mag_b / norm(mag_b); */
  b_value = norm(mag_b);
  for (i0 = 0; i0 < 3; i0++) {
    acc_b[i0] /= value;
    mag_b[i0] /= b_value;
  }

  /*  normalized row vector in NED */
  /*  Izracunavam kvaternion iz ziroskopa */
  /* 'comp_filt_step:10' qib_gyro = q_gyro(gyr_b, s.qib_prev, s.dt); */
  /*  Funkcija q_gyro() procijenjuje kvaternion iz ulaznih parametara: */
  /*        w_b = mjereni vektor kutne brzine u koordinatnom sustavu tijela */
  /*        qib_prev = prethodno procijenjeni kvaternion      */
  /*        dt = vremenski korak */
  /*  Funkcija q_gd() vraca: */
  /*        qib = estimirani kvaternion */
  /*  Racunam matricu za izracun vremenske promjene kvaterniona */
  /* 'q_gyro:11' S_w = [0     , -w_b(1), -w_b(2), -w_b(3); */
  /* 'q_gyro:12'        w_b(1),  0     ,  w_b(3), -w_b(2); */
  /* 'q_gyro:13'        w_b(2), -w_b(3),  0     ,  w_b(1); */
  /* 'q_gyro:14'        w_b(3),  w_b(2), -w_b(1),  0]; */
  /*  Racunam vremeski promjenu (rate of change) kvaterniona iz kutne brzine */
  /* 'q_gyro:17' d_qib_dt = 0.5*S_w*qib_prev; */
  /*  Integriram (akumuliram) promjenu kvaterniona i dodajem prethodnom kvaternionu         */
  /* 'q_gyro:20' qib = dt*d_qib_dt + qib_prev; */
  fv5[0] = 0.0F;
  fv5[4] = -gyr_b[0];
  fv5[8] = -gyr_b[1];
  fv5[12] = -gyr_b[2];
  fv5[1] = gyr_b[0];
  fv5[5] = 0.0F;
  fv5[9] = gyr_b[2];
  fv5[13] = -gyr_b[1];
  fv5[2] = gyr_b[1];
  fv5[6] = -gyr_b[2];
  fv5[10] = 0.0F;
  fv5[14] = gyr_b[0];
  fv5[3] = gyr_b[2];
  fv5[7] = gyr_b[1];
  fv5[11] = -gyr_b[0];
  fv5[15] = 0.0F;
  for (i0 = 0; i0 < 4; i0++) {
    for (i1 = 0; i1 < 4; i1++) {
      fv6[i1 + (i0 << 2)] = 0.5F * fv5[i1 + (i0 << 2)];
    }
  }

  for (i0 = 0; i0 < 4; i0++) {
    fv7[i0] = 0.0F;
    for (i1 = 0; i1 < 4; i1++) {
      fv7[i0] += fv6[i0 + (i1 << 2)] * s->qib_prev[i1];
    }

    qib_est[i0] = s->dt * fv7[i0] + s->qib_prev[i0];
  }

  /*  Normaliziram kvaternion */
  /* 'q_gyro:23' qib = qib/norm(qib); */
  value = b_norm(qib_est);

  /*  qib predstavlja procjenu kvaterniona u trenutnom koraku */
  /*  Racunam optimalni kvaternon iz acc i mag pomocu gradient descenta */
  /*  Za inicijalni kvaternion koristim onaj izracunat iz ziroskopa */
  /* 'comp_filt_step:14' [qib_gd, ~] = q_gd(acc_b, mag_b, s.acc_i, s.mag_i, qib_gyro, s.alpha, s.N_max, s.dJ_max); */
  /*  Funkcija q_gd() je iterativni estimator kvaterniona koja prima: */
  /*        a_b = mjereni vektor akceleracije u koordinatnom sustavu tijela */
  /*        m_b = mjereni vektor magnetskog polja u koordinatnom sustavu tijela      */
  /*        a_i = mjereni vektor akceleracije u inercijalnom koordinatnom sustavu */
  /*        m_i = mjereni vektor akceleracije u inercijalnom koordinatnom sustavu */
  /*        qib = inicijalni kvaternion za poèetak iteratovnog postupa te maksimalnu */
  /*        N_max = maksimalni broj iteracija */
  /*        dJ_max = dozvoljenu razliku u gresci */
  /*  Funkcija q_gd() vraca vektor koji sadrzi dva elementa: */
  /*        qib = estimirani kvaternion */
  /*        NOI = broj izvršenih itreracija */
  for (i = 0; i < 4; i++) {
    b_qib_est = qib_est[i] / value;
    qib_gd[i] = b_qib_est;
    qib_est[i] = b_qib_est;
  }

  /* 'q_gd:14' a_i = a_i/norm(a_i); */
  value = norm(s->acc_i);

  /*  Normaliziram akceleraciju */
  /* 'q_gd:15' m_i = m_i/norm(m_i); */
  b_value = norm(s->mag_i);

  /*  Normaliziram mag polje */
  /* 'q_gd:17' a_b = a_b/norm(a_b); */
  c_value = norm(acc_b);

  /*  Normaliziram akceleraciju */
  /* 'q_gd:18' m_b = m_b/norm(m_b); */
  d_value = norm(mag_b);
  for (i = 0; i < 3; i++) {
    a_i[i] = s->acc_i[i] / value;
    m_i[i] = s->mag_i[i] / b_value;
    a_b[i] = acc_b[i] / c_value;
    m_b[i] = mag_b[i] / d_value;
  }

  /*  Normaliziram mag polje */
  /* 'q_gd:20' J_previous = 0; */
  J_previous = 0.0F;

  /*  Prethodna vrijednost funkcije cilja (geldamo razliku) */
  /* 'q_gd:21' NOI = 0; */
  /*  Trenutni broj iteracija */
  /*  Pokrecem petlju s maksimalno N_max iteracija */
  /* 'q_gd:24' for i=1:N_max */
  i = 0;
  exitg1 = false;
  while ((!exitg1) && (i <= (int)s->N_max - 1)) {
    /*  Racunam rotacijsku matricu */
    /* 'q_gd:27' Rib = qib2Rib(qib); */
    /*  Funkcija qib2Rib() racuna rotacijsku matricu iz zadanog kvaterniona. */
    /*  Ulazni parametri: */
    /*        qib = zadani kvaternion (4D stupcasti vektor) */
    /*  Izlazni parametri: */
    /*        Rib = izracunata rotacijska matrica (3x3) */
    /*  Izracun elemenata rotacijske matrice iz kvaterniona */
    /* 'qib2Rib:10' r11 = qib(1)^2+qib(2)^2-qib(3)^2-qib(4)^2; */
    /* 'qib2Rib:11' r12 = 2*(qib(2)*qib(3)+qib(1)*qib(4)); */
    /* 'qib2Rib:12' r13 = 2*(qib(2)*qib(4)-qib(1)*qib(3)); */
    /* 'qib2Rib:14' r21 = 2*(qib(2)*qib(3)-qib(1)*qib(4)); */
    /* 'qib2Rib:15' r22 = qib(1)^2-qib(2)^2+qib(3)^2-qib(4)^2; */
    /* 'qib2Rib:16' r23 = 2*(qib(3)*qib(4)+qib(1)*qib(2)); */
    /* 'qib2Rib:18' r31 = 2*(qib(2)*qib(4)+qib(1)*qib(3)); */
    /* 'qib2Rib:19' r32 = 2*(qib(3)*qib(4)-qib(1)*qib(2)); */
    /* 'qib2Rib:20' r33 = qib(1)^2-qib(2)^2-qib(3)^2+qib(4)^2; */
    /*  Kreiranje rotacijske matrice I -> B od izracunatih elemenata */
    /* 'qib2Rib:23' Rib = [r11,r12,r13; */
    /* 'qib2Rib:24'        r21,r22,r23; */
    /* 'qib2Rib:25'        r31,r32,r33]; */
    Rib[0] = ((qib_gd[0] * qib_gd[0] + qib_gd[1] * qib_gd[1]) - qib_gd[2] *
              qib_gd[2]) - qib_gd[3] * qib_gd[3];
    Rib[3] = 2.0F * (qib_gd[1] * qib_gd[2] + qib_gd[0] * qib_gd[3]);
    Rib[6] = 2.0F * (qib_gd[1] * qib_gd[3] - qib_gd[0] * qib_gd[2]);
    Rib[1] = 2.0F * (qib_gd[1] * qib_gd[2] - qib_gd[0] * qib_gd[3]);
    Rib[4] = ((qib_gd[0] * qib_gd[0] - qib_gd[1] * qib_gd[1]) + qib_gd[2] *
              qib_gd[2]) - qib_gd[3] * qib_gd[3];
    Rib[7] = 2.0F * (qib_gd[2] * qib_gd[3] + qib_gd[0] * qib_gd[1]);
    Rib[2] = 2.0F * (qib_gd[1] * qib_gd[3] + qib_gd[0] * qib_gd[2]);
    Rib[5] = 2.0F * (qib_gd[2] * qib_gd[3] - qib_gd[0] * qib_gd[1]);
    Rib[8] = ((qib_gd[0] * qib_gd[0] - qib_gd[1] * qib_gd[1]) - qib_gd[2] *
              qib_gd[2]) + qib_gd[3] * qib_gd[3];

    /*  Racunam vektora greske */
    /* 'q_gd:30' e_a = Rib*a_i - a_b; */
    /* 'q_gd:31' e_m = Rib*m_i - m_b; */
    /*  Racunam vrijednost funckije cilja u i-toj iteraciji */
    /* 'q_gd:34' J_a = e_a'*e_a; */
    /* 'q_gd:35' J_m = e_m'*e_m; */
    /* 'q_gd:36' J_current = J_a + J_m; */
    c_value = 0.0F;
    d_value = 0.0F;
    for (i0 = 0; i0 < 3; i0++) {
      b_value = 0.0F;
      for (i1 = 0; i1 < 3; i1++) {
        b_value += Rib[i0 + 3 * i1] * a_i[i1];
      }

      e_a[i0] = b_value - a_b[i0];
      b_value = 0.0F;
      for (i1 = 0; i1 < 3; i1++) {
        b_value += Rib[i0 + 3 * i1] * m_i[i1];
      }

      e_m[i0] = b_value - m_b[i0];
      c_value += e_a[i0] * e_a[i0];
      d_value += e_m[i0] * e_m[i0];
    }

    J_current = c_value + d_value;

    /*  Provjeravam je li ispunjen uvjet za prekid iterativnog postupka */
    /* 'q_gd:39' if abs(J_current - J_previous) < dJ_max */
    if (fabsf(J_current - J_previous) < s->dJ_max) {
      exitg1 = true;
    } else {
      /*  Racunam gradijent funkcije cilja za trenutni kvaternion */
      /* 'q_gd:44' grad_J = grad(a_i, m_i, e_a, e_m, qib); */
      /*  Funkcija q_gd() je iterativni estimator kvaterniona koja prima:    */
      /*        a_i = mjereni vektor akceleracije u inercijalnom koordinatnom sustavu */
      /*        m_i = mjereni vektor akceleracije u inercijalnom koordinatnom sustavu */
      /*        e_a = vektor greške akcelerometra */
      /*        e_m = vektor greške magnetometra */
      /*        qib = zadani kvaternion u kojem raèunamo graijent */
      /*  Funkcija q_gd() vraca vektor koji sadrzi dva elementa: */
      /*        grad_J = gradijent funkcije cilja za zadani kvaternion */
      /*  Inicijaliziram gradijent funkcije cilja za zadani kvaternion */
      /* 'grad:13' grad_J = [0 0 0 0]'; */
      /*  Parcijalna derivacija rotacijske matrice Rib_gd po q1 */
      /* 'grad:16' M1 = 2*[ qib(1)  qib(4) -qib(3); */
      /* 'grad:17'         -qib(4)  qib(1)  qib(2); */
      /* 'grad:18'          qib(3) -qib(2)  qib(1)]; */
      b_qib_gd[0] = qib_gd[0];
      b_qib_gd[3] = qib_gd[3];
      b_qib_gd[6] = -qib_gd[2];
      b_qib_gd[1] = -qib_gd[3];
      b_qib_gd[4] = qib_gd[0];
      b_qib_gd[7] = qib_gd[1];
      b_qib_gd[2] = qib_gd[2];
      b_qib_gd[5] = -qib_gd[1];
      b_qib_gd[8] = qib_gd[0];

      /*  Parcijalna derivacija rotacijske matrice Rib_gd po q2 */
      /* 'grad:21' M2 = 2*[ qib(2)  qib(3)  qib(4); */
      /* 'grad:22'          qib(3) -qib(2)  qib(1); */
      /* 'grad:23'          qib(4) -qib(1) -qib(2)]; */
      c_qib_gd[0] = qib_gd[1];
      c_qib_gd[3] = qib_gd[2];
      c_qib_gd[6] = qib_gd[3];
      c_qib_gd[1] = qib_gd[2];
      c_qib_gd[4] = -qib_gd[1];
      c_qib_gd[7] = qib_gd[0];
      c_qib_gd[2] = qib_gd[3];
      c_qib_gd[5] = -qib_gd[0];
      c_qib_gd[8] = -qib_gd[1];

      /*  Parcijalna derivacija rotacijske matrice Rib_gd po q3 */
      /* 'grad:26' M3 = 2*[-qib(3)  qib(2) -qib(1); */
      /* 'grad:27'          qib(2)  qib(3)  qib(4); */
      /* 'grad:28'          qib(1)  qib(4) -qib(3)]; */
      d_qib_gd[0] = -qib_gd[2];
      d_qib_gd[3] = qib_gd[1];
      d_qib_gd[6] = -qib_gd[0];
      d_qib_gd[1] = qib_gd[1];
      d_qib_gd[4] = qib_gd[2];
      d_qib_gd[7] = qib_gd[3];
      d_qib_gd[2] = qib_gd[0];
      d_qib_gd[5] = qib_gd[3];
      d_qib_gd[8] = -qib_gd[2];

      /*  Parcijalna derivacija rotacijske matrice Rib_gd po q4 */
      /* 'grad:31' M4 = 2*[-qib(4)  qib(1)  qib(2); */
      /* 'grad:32'         -qib(1) -qib(4)  qib(3); */
      /* 'grad:33'          qib(2)  qib(3)  qib(4)]; */
      e_qib_gd[0] = -qib_gd[3];
      e_qib_gd[3] = qib_gd[0];
      e_qib_gd[6] = qib_gd[1];
      e_qib_gd[1] = -qib_gd[0];
      e_qib_gd[4] = -qib_gd[3];
      e_qib_gd[7] = qib_gd[2];
      e_qib_gd[2] = qib_gd[1];
      e_qib_gd[5] = qib_gd[2];
      e_qib_gd[8] = qib_gd[3];

      /*  Raèunam gradijent za zadani kvaternion */
      /* 'grad:36' grad_J(1) = 2*e_a'*M1*a_i  +  2*e_m'*M1*m_i; */
      c_value = 0.0F;
      d_value = 0.0F;
      for (i0 = 0; i0 < 3; i0++) {
        fv8[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
          Rib[i1 + 3 * i0] = 2.0F * b_qib_gd[i1 + 3 * i0];
          M2[i1 + 3 * i0] = 2.0F * c_qib_gd[i1 + 3 * i0];
          M3[i1 + 3 * i0] = 2.0F * d_qib_gd[i1 + 3 * i0];
          M4[i1 + 3 * i0] = 2.0F * e_qib_gd[i1 + 3 * i0];
          fv8[i0] += 2.0F * e_a[i1] * Rib[i1 + 3 * i0];
        }

        c_value += fv8[i0] * a_i[i0];
        fv9[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
          fv9[i0] += 2.0F * e_m[i1] * Rib[i1 + 3 * i0];
        }

        d_value += fv9[i0] * m_i[i0];
      }

      grad_J[0] = c_value + d_value;

      /* 'grad:37' grad_J(2) = 2*e_a'*M2*a_i  +  2*e_m'*M2*m_i; */
      c_value = 0.0F;
      d_value = 0.0F;
      for (i0 = 0; i0 < 3; i0++) {
        fv8[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
          fv8[i0] += 2.0F * e_a[i1] * M2[i1 + 3 * i0];
        }

        c_value += fv8[i0] * a_i[i0];
        fv9[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
          fv9[i0] += 2.0F * e_m[i1] * M2[i1 + 3 * i0];
        }

        d_value += fv9[i0] * m_i[i0];
      }

      grad_J[1] = c_value + d_value;

      /* 'grad:38' grad_J(3) = 2*e_a'*M3*a_i  +  2*e_m'*M3*m_i; */
      c_value = 0.0F;
      d_value = 0.0F;
      for (i0 = 0; i0 < 3; i0++) {
        fv8[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
          fv8[i0] += 2.0F * e_a[i1] * M3[i1 + 3 * i0];
        }

        c_value += fv8[i0] * a_i[i0];
        fv9[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
          fv9[i0] += 2.0F * e_m[i1] * M3[i1 + 3 * i0];
        }

        d_value += fv9[i0] * m_i[i0];
      }

      grad_J[2] = c_value + d_value;

      /* 'grad:39' grad_J(4) = 2*e_a'*M4*a_i  +  2*e_m'*M4*m_i; */
      c_value = 0.0F;
      d_value = 0.0F;
      for (i0 = 0; i0 < 3; i0++) {
        fv8[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
          fv8[i0] += 2.0F * e_a[i1] * M4[i1 + 3 * i0];
        }

        c_value += fv8[i0] * a_i[i0];
        fv9[i0] = 0.0F;
        for (i1 = 0; i1 < 3; i1++) {
          fv9[i0] += 2.0F * e_m[i1] * M4[i1 + 3 * i0];
        }

        d_value += fv9[i0] * m_i[i0];
      }

      grad_J[3] = c_value + d_value;

      /*  Primijenjujem gradient decent algoritam na svaki element kvaterniona */
      /* 'q_gd:47' qib = qib - alpha*grad_J; */
      for (i0 = 0; i0 < 4; i0++) {
        qib_gd[i0] -= s->alpha * grad_J[i0];
      }

      /*  Normaliziram izraèunati kvaternion */
      /* 'q_gd:50' qib = qib/norm(qib); */
      value = b_norm(qib_gd);
      for (i0 = 0; i0 < 4; i0++) {
        qib_gd[i0] /= value;
      }

      /*  Pripremam vrijednost cost funkcije za sljedecu iteraciju */
      /* 'q_gd:53' J_previous = J_current; */
      J_previous = J_current;

      /* 'q_gd:54' NOI = NOI + 1; */
      i++;
    }
  }

  /* 'comp_filt_step:14' ~ */
  /*  Komplementarni filtar */
  /* 'comp_filt_step:17' qib_est = s.K_cf * qib_gyro + (1 - s.K_cf) * qib_gd; */
  c_value = 1.0F - s->K_cf;
  for (i0 = 0; i0 < 4; i0++) {
    qib_est[i0] = s->K_cf * qib_est[i0] + c_value * qib_gd[i0];
  }

  /* 'comp_filt_step:18' qib_est = qib_est / norm(qib_est); */
  value = b_norm(qib_est);

  /*  Storing current quaternion for the next iterative process */
  /* 'comp_filt_step:21' s.qib_prev = qib_est; */
  for (i = 0; i < 4; i++) {
    b_qib_est = qib_est[i] / value;
    s->qib_prev[i] = b_qib_est;
    qib_est[i] = b_qib_est;
  }
}

/*
 * File trailer for comp_filt_step.c
 *
 * [EOF]
 */
