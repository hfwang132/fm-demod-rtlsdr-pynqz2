#ifndef _FIR_COMPLEX_H_
#define _FIR_COMPLEX_H_

// This is to resolve a bug regarding cosimulation with arbitrary precision data types.
// This bug is only present with Vitis HLS 2021.x
// See https://support.xilinx.com/s/article/Use-of-gmp-h-for-Co-simulation?language=en_US
#include <gmp.h>
#define __gmp_const const

#include "ap_int.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"

#define N 64

#define D 10

typedef ap_fixed<24, 12, AP_RND> data_t;
typedef ap_fixed<24, 12, AP_RND> coef_t;
typedef float fdata_t;
typedef float fcoef_t;

typedef struct {
	data_t i;
	data_t q;
} cdata_t ;
typedef struct {
	fdata_t i;
	fdata_t q;
} cfdata_t ;

typedef hls::axis<data_t, 0, 0, 0> pkt_data_t;
typedef hls::axis<cdata_t, 0, 0, 0> pkt_cdata_t;
typedef hls::axis<cfdata_t, 0, 0, 0> pkt_cfdata_t;

void fir_complex(hls::stream<pkt_cfdata_t>& out_stream, hls::stream<pkt_cfdata_t>& in_stream, fcoef_t *coef, int load_coef);

#endif
