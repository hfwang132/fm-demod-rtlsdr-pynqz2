# Introduction

This folder contains the source codes for Vitis HLS. 

There are three IPs: `fir_complex`, `fm_discrim`, and `fir_real`.

* These three IPs have some characteristics in common:
  * AXI-Stream Interface
  * Optimation directives:
    * Arbitrary precision data type
    * Pipelining
    * Coeffcients and data stored in BRAMs

* `fir_complex` is the first-stage two-channel (I/Q) decimation filter.
  * 64-tap FIR; Decimation factor = 10; Programmable coefficients
  * Resource utilization (Before implemention): 4298 FFs, 9519 LUTs, 2 DSPs, 9 BRAMs
  * Latency = 162 cycles (for 10 samples), not pipelined

* `fm_discrim` is the phase discrimination IP.
  * Resource utilization (Before implemention): 6722 FFs, 5363 LUTs, 0 DSPs, 0 BRAM
  * Latency = 52 cycles, pipelined with Initiation Interval (II) = 1

* `fir_real` is the second stage decimation filter.
  * 64-tap FIR; Decimation factor = 5; Programmable coefficients
  * Resource utilization (Before implementation): 3067 FFs, 5897 LUTs, 1 DSP, 7 BRAMs
  * Latency = 162 cycles (for 5 samples), not pipelined
  
* Throughput analysis
  * These three IPs are streamed in serial.
  * The sample rate is:
    * 2.4MSPS before the first filter
    * 240kSPS between the first and the second filter 
    * 48000SPS after the second filter
  * The throughput depend on the IP involving the max sample rate, which is the first filter `fir_complex`.
  * The throughput is $1\text{s}/[(162\cdot 10\text{ns})/10\text{samples}] = 6.17\text{MSPS} > 2.4\text{MSPS}$, which satisfies the requirement for real-time processing.

* Design Space Exploration
  * `LOW_AREA_LOW_THROUGHPUT`:
    * `LOW_AREA_LOW_THROUGHPUT` is a macro defined by default. Define it to use low-area-low-throughput optimization strategy.
    * Data are stored in BRAMs. TDL and MAC operations are implemented as pipelined, not paralled.

  * `HIGH_AREA_HIGH_THROUGHPUT`:
    * Array partitioning and loop unrolling can be used to exchange more areas for fewer latencies. However, this is not recommended as there is limited resources on xc7z020 devices.
    * In the `HIGH_AREA_HIGH_THROUGHPUT` implementation, the top loop of the FIR filters can be pipelined with II=1, achieving an astounding one-cycle-for-one-sample throughput. Under 10ns clock cycle, this means a throughput of 100MSPS. However, we only need the throughput be greater than 2.4MSPS. Also, the area consumption is too big because all the sub loops are completely unrolled.
    * If you want to use it, simply undefine `LOW_AREA_LOW_THROUGHPUT` and define `HIGH_AREA_HIGH_THROUGHPUT`.

# Caveats

Note that the following codes are added at the header files:

```c
#include <gmp.h>
#define __gmp_const const
```

This is to resolve the cosimulation bug when using arbitrary precision data types. 

See https://support.xilinx.com/s/article/Use-of-gmp-h-for-Co-simulation?language=en_US
