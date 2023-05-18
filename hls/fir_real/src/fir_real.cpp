#include "fir_real.h"

//#define HIGH_AREA_HIGH_THROUGHPUT
#define LOW_AREA_LOW_THROUGHPUT

void fir_real(hls::stream<pkt_fdata_t>& out_stream, hls::stream<pkt_fdata_t>& in_stream, fcoef_t *coef, int load_coef)
{
#pragma HLS INTERFACE s_axilite port=load_coef
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE m_axi depth=64 port=coef offset=slave
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE axis port=in_stream

    static data_t shift_reg[N] = {0};
    static data_t coef_buf[N] = {0};

#ifdef HIGH_AREA_HIGH_THROUGHPUT
#pragma HLS ARRAY_PARTITION variable=shift_reg dim=1 complete
#pragma HLS ARRAY_PARTITION variable=coef_buf dim=1 complete
#endif

    data_t acc;

    int k;

    pkt_fdata_t pkt;

    if (load_coef) {
    	copy_coef_loop:
    	for (k = 0; k < N; k ++) {
#ifdef HIGH_AREA_HIGH_THROUGHPUT
#pragma HLS PIPELINE II=1
#endif
			coef_buf[k] = (coef_t) coef[N - k - 1];
		}
    }

    top_loop:
	while (1) {
#pragma HLS LOOP_TRIPCOUNT avg=48000 max=96000 min=5

#ifdef HIGH_AREA_HIGH_THROUGHPUT
#pragma HLS PIPELINE II=5
#endif

		TDL: // completely unrolled if parent loop is pipelined
		for (k = 0; k < N - D; k ++) {
#ifdef LOW_AREA_LOW_THROUGHPUT
#pragma HLS PIPELINE II=1
#endif
			shift_reg[k] = shift_reg[k + D];
		}

		READ: // completely unrolled if parent loop is pipelined
		for (k = 0; k < D; k ++) {
#ifdef LOW_AREA_LOW_THROUGHPUT
#pragma HLS PIPELINE II=1
#endif
			in_stream.read(pkt);
			// float to fixed
			pkt.data = (data_t) pkt.data;
			shift_reg[N - D + k] = pkt.data;
		}

		acc = 0;

		MAC: // completely unrolled if parent loop is pipelined
		for (k = 0; k < N; k ++) {
#ifdef LOW_AREA_LOW_THROUGHPUT
#pragma HLS PIPELINE II=1
#endif
			acc += shift_reg[k] * coef_buf[k];
		}

		// fixed to float
		pkt.data = (float) acc;

		out_stream.write(pkt);

		if (pkt.last == 1) {
			break;
		}
    }
}
