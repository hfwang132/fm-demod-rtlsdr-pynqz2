# FM demodulation with PYNQ-Z2 and RTL-SDR

---

## 1) Introduction

This project provides an example of FM demodulation on PYNQ-Z2 with RTL-SDR. Both software and hardware implementations are explored.

## 2) Hardware Platform
  * PYNQ-Z2
  * RTL-SDR

## 3) Software Platform

  * Vivado 2022.2
  * Vitis HLS 2022.2
  * PYNQ v3.0.1

## 4) Performance

- Software implementation
  - takes ~4.6s to demodulate 2.4M samples
- Hardware implementation
  - takes ~321ms to demodulate 2.4M samples

## 5) Reference

[Parallel Programming for FPGAs](https://pp4fpgas.readthedocs.io/en/latest/project7.html) by Kastner Research Group.
