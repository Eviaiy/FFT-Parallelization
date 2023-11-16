/*
 * The provided CUDA code example utilizes NVIDIA's cuFFT library, an optimized and
 * high-performance library for computing FFTs on NVIDIA GPUs. This library abstracts
 * the specifics of the FFT algorithm, such as Cooley-Tukey, focusing instead on providing
 * efficient FFT computations regardless of the underlying algorithmic details.
 *
 * In most FFT libraries, including cuFFT, the exact variant of the FFT algorithm (like
 * Cooley-Tukey, Radix-2, Radix-4, Split-Radix, etc.) used internally is not explicitly
 * specified. This is because these libraries are designed to automatically choose the most
 * efficient algorithm and implementation based on the input size and the capabilities of
 * the hardware being used. As a result, while using such libraries, the user does not need
 * to worry about the specifics of the FFT algorithm implementations. The library handles
 * the selection and optimization of the algorithm, ensuring the best possible performance
 * for FFT computations on the given hardware.
 */

#include <cufft.h>
#include <stdio.h>

#define NX 256  // Size of the FFT


int main() {
    cufftHandle plan;
    cufftComplex *data, *d_data;
    int mem_size = sizeof(cufftComplex) * NX;

    // Allocate host memory for the signal
    data = (cufftComplex *)malloc(mem_size);

    // Initialize the memory for the signal
    for (int i = 0; i < NX; i++) {
        data[i].x = i % 8;  // Sample data
        data[i].y = 0;
    }

    // Allocate device memory for signal
    cudaMalloc((void **)&d_data, mem_size);

    // Copy host memory to device
    cudaMemcpy(d_data, data, mem_size, cudaMemcpyHostToDevice);

    // Create a 1D FFT plan
    cufftPlan1d(&plan, NX, CUFFT_C2C, 1);

    // Execute the plan forward
    cufftExecC2C(plan, d_data, d_data, CUFFT_FORWARD);

    // Copy transformed data back to host
    cudaMemcpy(data, d_data, mem_size, cudaMemcpyDeviceToHost);

    // Print a few elements of the result
    for (int i = 0; i < 10; i++) {
        printf("%f + %fi\n", data[i].x, data[i].y);
    }

    // Cleanup
    cufftDestroy(plan);
    cudaFree(d_data);
    free(data);

    return 0;
}
