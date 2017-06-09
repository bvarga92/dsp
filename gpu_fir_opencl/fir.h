#ifndef _FIR_H_
#define _FIR_H_

	#define SAMPLE_NUM         4096 //mintaszam
	#define TAP_NUM             128 //FIR szuro egyutthatoinak szama
	#define MAC_UNROLL_FACTOR    32 //MAC ciklus unroll faktora (TAP_NUM osztoja!)
	#define BLOCK_SIZE          128 //thread block merete (SAMPLE_NUM osztoja)
	#define CPU_RUNS           5000 //ennyiszer futtatjuk a CPU fuggvenyt
	#define GPU_RUNS          10000 //ennyiszer futtatjuk a GPU kerneleket

#endif
