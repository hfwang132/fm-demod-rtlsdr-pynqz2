#include "fir_complex.h"

//#define HIGH_AREA_HIGH_THROUGHPUT
#define LOW_AREA_LOW_THROUGHPUT

void fir_complex(hls::stream<pkt_cfdata_t>& out_stream, hls::stream<pkt_cfdata_t>& in_stream, fcoef_t *coef, int load_coef)
{
#pragma HLS INTERFACE s_axilite port=load_coef
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE m_axi depth=64 port=coef offset=slave
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE axis port=in_stream

    static data_t shift_reg_i[N] = {0};
    static data_t shift_reg_q[N] = {0};
    static data_t coef_buf[N] = {0};

#ifdef HIGH_AREA_HIGH_THROUGHPUT
#pragma HLS ARRAY_PARTITION variable=shift_reg_i dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=shift_reg_q dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=coef_buf dim=1 type=complete
#endif

    data_t acc_i;
    data_t acc_q;

    int k;

    pkt_cfdata_t pkt;

    if (load_coef) {
    	copy_coef:
    	for (k = 0; k < N; k ++) {
			coef_buf[k] = (coef_t) coef[k];
		}
    }

    top_loop:
	while (1) {
#pragma HLS LOOP_TRIPCOUNT max=480000 avg=240000 min=1

#ifdef HIGH_AREA_HIGH_THROUGHPUT
#pragma HLS PIPELINE II=10
#endif
		TDL: // completely unrolled if parent loop is pipelined
		for (k = N - 1; k >= D; k --) {
#ifdef LOW_AREA_LOW_THROUGHPUT
#pragma HLS PIPELINE II=1
#endif
			shift_reg_i[k] = shift_reg_i[k - D];
			shift_reg_q[k] = shift_reg_q[k - D];
		}

		READ: // completely unrolled if parent loop is pipelined
		for (k = 0; k < D; k ++) {
#ifdef LOW_AREA_LOW_THROUGHPUT
#pragma HLS PIPELINE II=1
#endif
			in_stream.read(pkt);
			// float to fixed
			pkt.data.i = (data_t) pkt.data.i;
			pkt.data.q = (data_t) pkt.data.q;
			shift_reg_i[D - k - 1] = pkt.data.i;
			shift_reg_q[D - k - 1] = pkt.data.q;
		}

		acc_i = 0;
		acc_q = 0;

		MAC: // completely unrolled if parent loop is pipelined
		for (k = N - 1; k >= 0; k--) {
#ifdef LOW_AREA_LOW_THROUGHPUT
#pragma HLS PIPELINE II=1
#endif
			acc_i += shift_reg_i[k] * coef_buf[k];
			acc_q += shift_reg_q[k] * coef_buf[k];
		}

		// fixed to float
		pkt.data.i = (fdata_t) acc_i;
		pkt.data.q = (fdata_t) acc_q;

		out_stream.write(pkt);

		if (pkt.last) {
			break;
		}
    }
}

