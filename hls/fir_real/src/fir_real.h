#ifndef _FIR_REAL_H_
#define _FIR_REAL_H_

// This is to resolve a bug about cosimulation with arbitrary precision data types.
// This bug is only present with Vitis HLS 2021.x
// See https://support.xilinx.com/s/article/Use-of-gmp-h-for-Co-simulation?language=en_US
#include <gmp.h>
#define __gmp_const const

#include "ap_int.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"

#define N 64

#define D 5

typedef ap_fixed<24, 12, AP_RND> data_t;
typedef ap_fixed<24, 12, AP_RND> coef_t;
typedef float fdata_t;
typedef float fcoef_t;

typedef hls::axis<data_t, 0, 0, 0> pkt_data_t;
typedef hls::axis<fdata_t, 0, 0, 0> pkt_fdata_t;

void fir_real(hls::stream<pkt_fdata_t>& out_stream, hls::stream<pkt_fdata_t>& in_stream, fcoef_t *coef, int load_coef);

#endif
