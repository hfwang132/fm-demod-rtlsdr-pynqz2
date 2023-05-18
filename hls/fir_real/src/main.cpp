#include "fir_real.h"
#include <iostream>

int tb_fir_real();

int main()
{
	int err = 0;

	std::cout << " ****************** " << std::endl;
	std::cout << "Testing fir_real()" << std::endl;
	std::cout << " ****************** " << std::endl;
	err = tb_fir_real();
	if (err) {
		return err;
	}

	return 0;
}

int tb_fir_real()
{
	int err = 0;

	fdata_t x[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	hls::stream<pkt_fdata_t> in_stream;
	hls::stream<pkt_fdata_t> out_stream;

	pkt_fdata_t pkt_in;
	pkt_fdata_t pkt_out;

	fdata_t out_data;

	std::cout << "Writing sample to in_stream ..." << std::endl;

	for (int k = 0; k < 10; k++) {
		pkt_in.data = x[k];
		if (k < 9) {
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

	std::cout << "Streaming in_stream to fir_real() ..." << std::endl;

	fir_real(out_stream, in_stream, lpf_b, 1);

	std::cout << "Reading from out_stream ..." << std::endl;

	fdata_t golden[2] = {-0.0114746, 1.89697};

	for (int k = 0; k < 2; k++) {
		out_stream.read(pkt_out);
		out_data = pkt_out.data;

		std::cout << "golden: " << golden[k] << ",\t output: "
				<< out_data << std::endl;
	}

	return err;
}
