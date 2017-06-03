#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "fir.cuh"

void firCPU(float *input, float *output, float *coeffs, int samples, int taps){
	clock_t s0, e0;
	s0=clock();
	for(int i=0;i<CPU_RUNS;i++){
		for(int out_addr=0;out_addr<SAMPLE_NUM;out_addr++){
			float acc=0.0f;
			for(int tap=taps-1;tap>=0;tap--){
				int in_addr=out_addr-tap;
				if(in_addr>=0) acc+=coeffs[tap]*input[in_addr];
			}
			output[out_addr]=acc;
		}
	}
	e0=clock();
	printf("Execution time with CPU: %.1f us\n",(double)(e0-s0)*1000000.0/(CLOCKS_PER_SEC*CPU_RUNS));
}

void firCUDA(float *input, float *output, float *coeffs, int samples, int taps, bool useShared){
	/* ha tobb GPU van, akkor az elsot valasztjuk */
	cudaSetDevice(0);
	/* kernel parameterek */
	float *device_input, *device_coeffs, *device_output;
	cudaMalloc((void**)&device_input,samples*sizeof(float));
	cudaMalloc((void**)&device_coeffs,taps*sizeof(float));
	cudaMalloc((void**)&device_output,samples*sizeof(float));
	cudaMemcpy(device_input,input,samples*sizeof(float),cudaMemcpyHostToDevice); 
	cudaMemcpy(device_coeffs,coeffs,taps*sizeof(float),cudaMemcpyHostToDevice);
	/* ha az L1 cache es a megosztott memoria kozos, akkor a nagyobb megosztott memoriat preferaljuk */
	cudaDeviceSetCacheConfig(cudaFuncCachePreferShared);
	/* kernel futtatasa es idomeres */
	dim3 thrBlock(BLOCK_SIZE);
	dim3 thrGrid(samples/BLOCK_SIZE);
	clock_t s0, e0;
	if(useShared){
		s0=clock();
		for(int i=0;i<GPU_RUNS;i++) fir_kernel_sh<<<thrGrid,thrBlock>>>(device_input,device_coeffs,device_output,taps);
		cudaThreadSynchronize();
		e0=clock();
		printf("Execution time with GPU (using shared memory): %.1f us\n",(double)(e0-s0)*1000000.0/(CLOCKS_PER_SEC*GPU_RUNS));
	}
	else{
		s0=clock();
		for(int i=0;i<GPU_RUNS;i++) fir_kernel<<<thrGrid,thrBlock>>>(device_input,device_coeffs,device_output,taps);
		cudaThreadSynchronize();
		e0=clock();
		printf("Execution time with GPU (basic): %.1f us\n",(double)(e0-s0)*1000000.0/(CLOCKS_PER_SEC*GPU_RUNS));
	}
	/* eredmeny masolasa, befejezes */
	cudaMemcpy(output,device_output,samples*sizeof(float),cudaMemcpyDeviceToHost);
	cudaFree(device_input);
	cudaFree(device_coeffs); 
	cudaFree(device_output);
	cudaDeviceReset();
}

int main(){
	/* jel es szuroegyutthatok inicializalasa veletlenszamokkal */
	float *coeffs, *input, *output_cpu, *output_gpu_basic, *output_gpu_shared;
	coeffs=(float*)_aligned_malloc(TAP_NUM*sizeof(float),32);
	input=(float*)_aligned_malloc(SAMPLE_NUM*sizeof(float),32);
	output_cpu=(float*)_aligned_malloc(SAMPLE_NUM*sizeof(float),32);
	output_gpu_basic=(float*)_aligned_malloc(SAMPLE_NUM*sizeof(float),32);
	output_gpu_shared=(float*)_aligned_malloc(SAMPLE_NUM*sizeof(float),32);
	for(int i=0;i<TAP_NUM;i++){
		coeffs[i]=rand()/(float)RAND_MAX;
	}
	for(int i=0;i<SAMPLE_NUM;i++){
		input[i]=rand()/(float)RAND_MAX;
		output_cpu[i]=output_gpu_basic[i]=output_gpu_shared[i]=0.0f;
	}
	/* szures */
	firCPU(input,output_cpu,coeffs,SAMPLE_NUM,TAP_NUM);
	firCUDA(input,output_gpu_basic,coeffs,SAMPLE_NUM,TAP_NUM,false);
	firCUDA(input,output_gpu_shared,coeffs,SAMPLE_NUM,TAP_NUM,true);
	/* ellenorzes */
	double maxerror_basic=0.0, maxerror_shared=0.0;
	for(int i=0;i<SAMPLE_NUM;i++){
		double e=(double)output_cpu[i]-(double)output_gpu_basic[i];
		if(e<0) e=-e;
		if(e>maxerror_basic) maxerror_basic=e;
		e=(double)output_cpu[i]-(double)output_gpu_shared[i];
		if(e<0) e=-e;
		if(e>maxerror_shared) maxerror_shared=e;
	}
	printf("\nMax. error (basic): %.3e\nMax. error (shared memory): %.3e\n",maxerror_basic,maxerror_shared);
	/* befejezes */
	_aligned_free(coeffs);
	_aligned_free(input);
	_aligned_free(output_cpu);
	_aligned_free(output_gpu_basic);
	_aligned_free(output_gpu_shared);
	getchar();
	return 0;
}
