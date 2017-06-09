#include "fir.h"

__kernel void fir_kernel(__global float* input, __constant float* coeffs, __global float* output, int taps){
	int out_addr=get_group_id(0)*get_local_size(0)+get_local_id(0);
	float acc=0.0f;
	#pragma unroll MAC_UNROLL_FACTOR
	for(int tap=taps-1;tap>=0;tap--){
		int in_addr=out_addr-tap;
		if(in_addr>=0) acc=mad(coeffs[tap],input[in_addr],acc);
	}
	output[out_addr]=acc;
}

__kernel void fir_kernel_sh(__global float* input, __constant float* coeffs, __global float* output, int taps){
	int out_addr=get_group_id(0)*get_local_size(0)+get_local_id(0);
	__local float shmem[BLOCK_SIZE];
	shmem[get_local_id(0)]=input[out_addr];
	barrier(CLK_LOCAL_MEM_FENCE);
	float acc=0.0f;
	#pragma unroll MAC_UNROLL_FACTOR
	for(int tap=taps-1;tap>=0;tap--){
		int in_addr=out_addr-tap;
		if(in_addr>=0){
			if(get_local_id(0)>=tap)
				acc=mad(coeffs[tap],shmem[get_local_id(0)-tap],acc);
			else
				acc=mad(coeffs[tap],input[in_addr],acc);
		}
	}
	output[out_addr]=acc;
}
