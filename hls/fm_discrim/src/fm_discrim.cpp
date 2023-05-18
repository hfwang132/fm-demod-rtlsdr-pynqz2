#include <cstdio>
#include "fm_discrim.h"
#include "hls_stream.h"

void fm_discrim(hls::stream<pkt_fdata_t>& out_stream, hls::stream<pkt_cfdata_t>& in_stream)
{
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS TOP name=fm_discrim
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE axis port=in_stream

	pkt_cfdata_t pkt_in;
	pkt_fdata_t pkt_out;
	data_t i0, q0, i1, q1, di, dq;
	data_t numerator, denominator;
	i0 = 0;
	q0 = 0;

	while_loop: while (1) {
#pragma HLS LOOP_TRIPCOUNT max=480000 min=1 avg=240000
#pragma HLS PIPELINE II=1
		in_stream.read(pkt_in);
		// float to fixed

		i1 = (data_t) pkt_in.data.i;
		q1 = (data_t) pkt_in.data.q;

		di = i1 - i0;
		dq = q1 - q0;

		numerator = i1 * dq;
		numerator -= q1 * di;

		denominator = i1 * i1;
		denominator += q1 * q1;
		denominator += (data_t) 0.001;

		// fixed to float
		pkt_out.data = (fdata_t) (numerator / denominator);
//		don't write like this:
//		pkt_out.data = (fdata_t) ((i1 * dq - q1 * di) / (i1 * i1 + q1 * q1 + (data_t) 0.001));

		pkt_out.last = pkt_in.last;
		pkt_out.keep = pkt_in.keep;
		pkt_out.strb = pkt_in.strb;

		out_stream.write(pkt_out);

		i0 = i1;
		q0 = q1;

		if (pkt_in.last) {
			break;
		}
	}
}

