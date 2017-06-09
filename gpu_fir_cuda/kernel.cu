#include <stdio.h>
#include <time.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "fir.cuh"

__global__ void fir_kernel(const float* input, const float* coeffs, float* output, int taps){
	int out_addr=blockIdx.x*blockDim.x+threadIdx.x;
	float acc=0.0f;
	#pragma unroll MAC_UNROLL_FACTOR
	for(int tap=taps-1;tap>=0;tap--){
		int in_addr=out_addr-tap;
		if(in_addr>=0) acc=fmaf(coeffs[tap],input[in_addr],acc);
	}
	output[out_addr]=acc;
}

__global__ void fir_kernel_sh(const float* input, const float* coeffs, float* output, int taps){
	int out_addr=blockIdx.x*blockDim.x+threadIdx.x;
	__shared__ float shmem[BLOCK_SIZE];
	shmem[threadIdx.x]=input[out_addr];
	__syncthreads();
	float acc=0.0f;
	#pragma unroll MAC_UNROLL_FACTOR
	for(int tap=taps-1;tap>=0;tap--){
		int in_addr=out_addr-tap;
		if(in_addr>=0){
			if(threadIdx.x>=tap)
				acc=fmaf(coeffs[tap],shmem[threadIdx.x-tap],acc);
			else
				acc=fmaf(coeffs[tap],input[in_addr],acc);
		}
	}
	output[out_addr]=acc;
}
