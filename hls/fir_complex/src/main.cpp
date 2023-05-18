#include "fir_complex.h"
#include <iostream>

int tb_fir_complex();

int main()
{
	int err = 0;

	std::cout << " ****************** " << std::endl;
	std::cout << "Testing fir_complex()" << std::endl;
	std::cout << " ****************** " << std::endl;

	err = tb_fir_complex();
	if (err) {
		return err;
	}

	return err;
}

int tb_fir_complex()
{
	int err = 0;

	fdata_t i[20] = {
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			10, 9, 8, 7, 6, 5, 4, 3, 2, 1
	};
	fdata_t q[20] = {
			10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	};

	hls::stream<pkt_cfdata_t> in_stream;
	hls::stream<pkt_cfdata_t> out_stream;

	pkt_cfdata_t pkt_in;
	pkt_cfdata_t pkt_out;

	fdata_t out_i;
	fdata_t out_q;

	std::cout << "Writing sample to in_stream ..." << std::endl;

	for (int k = 0; k < 20; k++) {
		pkt_in.data.i = i[k];
		pkt_in.data.q = q[k];
		if (k < 19) {
			pkt_in.last = 0;
		} else {
			pkt_in.last = 1;
		}
		pkt_in.keep = 1;
		pkt_in.strb = 1;
		std::cout << "TLAST is " << (pkt_in.last == 1) <<
					" at " << k << " th sample." << std::endl;
		in_stream.write(pkt_in);
	}

	std::cout << "Streaming in_stream to fir_real() ..." << std::endl;

	fcoef_t lpf_b[64] = {
			-0.05709761, -0.02220113,  0.02486028,  0.07922919,  0.12968063,
			0.15763331,  0.1410031 ,  0.06301758, -0.07662158, -0.25299419,
		   -0.41485692, -0.49459031, -0.4288253 , -0.18478149,  0.21636375,
			0.68894149,  1.09266229,  1.26487516,  1.06975159,  0.45194196,
		   -0.52178765, -1.64869338, -2.61375893, -3.05080139, -2.62952764,
		   -1.14786505,  1.39521867,  4.76844932,  8.53121715, 12.10975284,
		   14.9111154 , 16.44868885, 16.44868885, 14.9111154 , 12.10975284,
			8.53121715,  4.76844932,  1.39521867, -1.14786505, -2.62952764,
		   -3.05080139, -2.61375893, -1.64869338, -0.52178765,  0.45194196,
			1.06975159,  1.26487516,  1.09266229,  0.68894149,  0.21636375,
		   -0.18478149, -0.4288253 , -0.49459031, -0.41485692, -0.25299419,
		   -0.07662158,  0.06301758,  0.1410031 ,  0.15763331,  0.12968063,
			0.07922919,  0.02486028, -0.02220113, -0.05709761
	};

	fir_complex(out_stream, in_stream, lpf_b, 1);

	std::cout << "Reading from out_stream ..." << std::endl;

	fdata_t golden_i[2] = {1.89697, 2.33447};
	fdata_t golden_q[2] = {0.157471, 35.5989};

	for (int k = 0; k < 2; k++) {
		out_stream.read(pkt_out);
		out_i = pkt_out.data.i;
		out_q = pkt_out.data.q;

		std::cout << "I golden: " << golden_i[k] << ",\t I output: "
				<< out_i << std::endl;

		std::cout << "Q golden: " << golden_q[k] << ",\t Q output: "
				<< out_q << std::endl;
	}

	return err;
}
