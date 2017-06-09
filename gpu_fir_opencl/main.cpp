#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <CL\cl.h>
#include "fir.h"

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

void firOpenCL(float *input, float *output, float *coeffs, int samples, int taps, bool useShared){
	cl_platform_id *platforms, platform;
	cl_device_id *devices, device;
	cl_uint numPlatforms, numDevices;
	size_t size, globalWorkSize, localWorkSize;
	char *str;
	cl_context context;
	FILE *fp;
	cl_program program;
	cl_kernel kernel;
	cl_mem device_input, device_coeffs, device_output;
	cl_command_queue queue;
	cl_event events[GPU_RUNS];
	cl_ulong timeStart, timeEnd;
	/* platform */
	clGetPlatformIDs(0,NULL,&numPlatforms);
	if(numPlatforms==0){
		printf("Error: no platforms found.\n");
		exit(1);
	}
	platforms=(cl_platform_id*)malloc(sizeof(cl_platform_id)*numPlatforms);
	clGetPlatformIDs(numPlatforms,platforms,NULL);
	platform=platforms[0];
	free(platforms);
	clGetPlatformInfo(platform,CL_PLATFORM_NAME,NULL,NULL,&size);
	str=(char*)malloc(size);
	clGetPlatformInfo(platform,CL_PLATFORM_NAME,size,str,NULL);
	//printf("Using platform: %s\n",str);
	free(str);
	/* device */
	clGetDeviceIDs(platform,CL_DEVICE_TYPE_ALL,0,NULL,&numDevices);
	if(numDevices==0){
		printf("Error: no devices found.\n");
		exit(1);
	}
	devices=(cl_device_id*)malloc(sizeof(cl_device_id)*numDevices);
	clGetDeviceIDs(platform,CL_DEVICE_TYPE_ALL,numDevices,devices,NULL);
	device=devices[0];
	free(devices);
	clGetDeviceInfo(device,CL_DEVICE_NAME,NULL,NULL,&size);
	str=(char*)malloc(size);
	clGetDeviceInfo(device,CL_DEVICE_NAME,size,str,NULL);
	//printf("Using device: %s\n",str);
	free(str);
	/* kernel program */
	context=clCreateContext(NULL,1,&device,NULL,NULL,NULL);
	fp=fopen("_src/kernel.cl","rb");
	if(!fp){
		printf("Failed to open kernel source file.\n");
		exit(1);
	}
	fseek(fp,0,SEEK_END);
	size=(size_t)ftell(fp);
	rewind(fp);
	str=(char*)malloc(size);
	fread(str,1,size,fp);
	fclose(fp);
	program=clCreateProgramWithSource(context,1,(const char**)&str,(const size_t*)&size,NULL);
	if(clBuildProgram(program,1,&device,"-I _src",NULL,NULL)!=CL_SUCCESS){
		clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,NULL,NULL,&size);
		str=(char*)realloc(str,size);
		clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,size,str,NULL);
		printf("Error building kernel: %s",str);
		free(str);
		exit(1);
	}
	free(str);
	kernel=useShared?clCreateKernel(program,"fir_kernel_sh",NULL):clCreateKernel(program,"fir_kernel",NULL);
	/* kernel parameterek atadasa */
	device_input=clCreateBuffer(context,CL_MEM_READ_ONLY,samples*sizeof(float),NULL,NULL);
	device_coeffs=clCreateBuffer(context,CL_MEM_READ_ONLY,taps*sizeof(float),NULL,NULL);
	device_output=clCreateBuffer(context,CL_MEM_WRITE_ONLY,samples*sizeof(float),NULL,NULL);
	queue=clCreateCommandQueue(context,device,CL_QUEUE_PROFILING_ENABLE,NULL);
	clEnqueueWriteBuffer(queue,device_input,CL_TRUE,0,samples*sizeof(float),input,0,NULL,NULL);
	clEnqueueWriteBuffer(queue,device_coeffs,CL_TRUE,0,taps*sizeof(float),coeffs,0,NULL,NULL);
	clSetKernelArg(kernel,0,sizeof(device_input),&device_input);
	clSetKernelArg(kernel,1,sizeof(device_coeffs),&device_coeffs);
	clSetKernelArg(kernel,2,sizeof(device_output),&device_output);
	clSetKernelArg(kernel,3,sizeof(int),&taps);
	clFinish(queue);
	/* kernel futtatasa */
	globalWorkSize=samples;
	localWorkSize=BLOCK_SIZE;
	for(unsigned i=0;i<GPU_RUNS;i++) clEnqueueNDRangeKernel(queue,kernel,1,NULL,&globalWorkSize,&localWorkSize,0,NULL,&events[i]);
	clWaitForEvents(1,&events[GPU_RUNS-1]);
	clGetEventProfilingInfo(events[0],CL_PROFILING_COMMAND_START,sizeof(timeStart),&timeStart,NULL);
	clGetEventProfilingInfo(events[GPU_RUNS-1],CL_PROFILING_COMMAND_END,sizeof(timeEnd),&timeEnd,NULL);
	if(useShared)
		printf("Execution time with GPU (using shared memory): %.1f us\n",(double)(timeEnd-timeStart)/1000.0/GPU_RUNS);
	else
		printf("Execution time with GPU (basic): %.1f us\n",(double)(timeEnd-timeStart)/1000.0/GPU_RUNS);
	/* eredmeny kiolvasasa */
	clEnqueueReadBuffer(queue,device_output,CL_TRUE,0,samples*sizeof(float),output,0,NULL,NULL);
	/* befejezes */
	clFlush(queue);
	clFinish(queue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseMemObject(device_input);
	clReleaseMemObject(device_coeffs);
	clReleaseMemObject(device_output);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
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
	firOpenCL(input,output_gpu_basic,coeffs,SAMPLE_NUM,TAP_NUM,false);
	firOpenCL(input,output_gpu_shared,coeffs,SAMPLE_NUM,TAP_NUM,true);
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
