# FM demodulation with AD9361, python and PYNQ

---

## 1) Introduction

This project provides an example of FM demodulation with AD9361, python and PYNQ. Both software and hardware implementations are explored.

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
