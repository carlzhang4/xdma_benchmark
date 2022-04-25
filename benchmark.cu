#include "benchmark.cuh"

#define ErrCheck(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

__global__ void write_bridge_gpu(volatile unsigned int* bridge, size_t length){
	int index = threadIdx.x;
	int iteration_time = length/64;
	__syncthreads();
	if(index<16){
		for(int i=0;i<iteration_time;i++){
			bridge[index + 16*i] = index+64;	
		}
	}
	__syncthreads();
}




void gpu_test(void* bridge){
	void * addr = (void*)bridge;
	size_t length = 512*1024;
	cudaError_t cudaerr = cudaHostRegister((void*)addr,length,cudaHostRegisterIoMemory);
	ErrCheck(cudaerr);
	unsigned int * dev_addr;
	cudaerr = cudaHostGetDevicePointer((void **) &(dev_addr),addr, 0);
	ErrCheck(cudaerr);

	write_bridge_gpu <<<1,16>>> (dev_addr, length);
	cudaDeviceSynchronize();
	cudaerr = cudaPeekAtLastError();
	ErrCheck(cudaerr);

	
}