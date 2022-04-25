/*
 * Copyright 2019 - 2020, RC4ML, Zhejiang University
 *
 * This hardware operator is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <immintrin.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <gdrapi.h>
#include <XDMA.h>
#include <XDMAController.h>


#include <fstream>
#include <iostream>
#include "benchmark.cuh"
extern "C"

using namespace std;

int main(int argc, char *argv[]) {
	fpga::XDMAController* dma_controller = fpga::XDMA::getController(1,0);//has_bypass, is_gpu_tlb, page_table

	void* bridge_void = dma_controller->get_bridge_addr();
	// gpu_test(bridge_void);

	size_t size = 512*1024;//byte

	cout<<hex<<bridge_void<<endl;

	if (mlock(bridge_void, size)) {
		printf("mlock error\n");
	}

	volatile __m512i* bridge = (__m512i*)bridge_void;

	__m512i data;
	for(int i=0;i<8;i++){
		data[i] = i;
	}

	struct timespec start_timer,end_timer;
	clock_gettime(CLOCK_MONOTONIC, &start_timer); 

	for(int i=0;i<size/64;i++){
		_mm512_store_epi64(bridge_void,data);
	}

	clock_gettime(CLOCK_MONOTONIC, &end_timer); 
	double time = (end_timer.tv_sec-start_timer.tv_sec)+1.0*(end_timer.tv_nsec-start_timer.tv_nsec)/1e9;
	printf("time:%f s\n",time);
	printf("speed:%f GB/s\n",size/time/1024/1024/1024);

	fpga::XDMA::clear();
	return 0;
}
