#include "fm_discrim.h"
#include <iostream>

int tb_fm_discrim();

int main()
{
	int err = 0;

	std::cout << " ****************** \n";
	std::cout << "Testing fm_discrim() \n";
	std::cout << " ****************** \n" << std::endl;

	err = tb_fm_discrim();
	if (err) {
		return err;
	}

	return err;
}


int tb_fm_discrim()
{
	int err = 0;

	cfdata_t iq[10];
	fdata_t i[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	fdata_t q[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

	for (int k = 0; k < 10; k ++) {
		iq[k].i = i[k];
		iq[k].q = q[k];
	}

	hls::stream<pkt_cfdata_t> in_stream;
	hls::stream<pkt_fdata_t> out_stream;

	pkt_cfdata_t pkt_in;
	pkt_fdata_t pkt_out;

	fdata_t out_data;

	std::cout << "Writing sample to in_stream ...\n";

	for (int k = 0; k < 10; k++) {
		pkt_in.data = iq[k];
		if (k < 9) {
			pkt_in.last = 0;
		} else {
			pkt_in.last = 1;
		}
		pkt_in.keep = 1;
		pkt_in.strb = 1;
		std::cout << "TLAST is " << (pkt_in.last==1) << " at " << k << " th sample.\n";
		in_stream.write(pkt_in);
	}

	printf("Streaming in_stream to fir_real() ...\n");

	fm_discrim(out_stream, in_stream);

	printf("Reading from out_stream ...\n");

	fdata_t golden[10] = {
			0.000000 , -0.129412, -0.150685, -0.169231, -0.180328,
			-0.180328, -0.169231, -0.150685, -0.129412, -0.108911
			};
	for (int k = 0; k < 10; k++) {
		out_stream.read(pkt_out);
		out_data = pkt_out.data;

		std::cout << "golden: " << golden[k] << ",\t output: " << out_data << std::endl;
	}

	return err;
}
