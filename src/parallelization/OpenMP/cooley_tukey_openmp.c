#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <omp.h>

// Function to perform the Cooley-Tukey FFT
void cooleyTukeyFFT(complex *X, int N, int stride) {
    if (N == 1) return;

    // Recursive calls
    cooleyTukeyFFT(X, N / 2, 2 * stride);
    cooleyTukeyFFT(X + stride, N / 2, 2 * stride);

    for (int k = 0; k < N / 2; k++) {
        complex t = cexp(-I * M_PI * k / N) * X[k * 2 * stride + stride];
        X[k * 2 * stride + stride] = X[k * 2 * stride] - t;
        X[k * 2 * stride] = X[k * 2 * stride] + t;
    }
}

int main() {
    int N = 1024;  // FFT size
    complex *data = (complex *)malloc(N * sizeof(complex));

    // Initialize the data array with sample values
    for (int i = 0; i < N; i++) {
        data[i] = cos(2 * M_PI * i / N) + I * sin(2 * M_PI * i / N);
    }

    // Parallel section
    #pragma omp parallel
    {
        #pragma omp single
        {
            cooleyTukeyFFT(data, N, 1);
        }
    }

    // Display the results
    for (int i = 0; i < N; i++) {
        printf("%lf + %lfi\n", creal(data[i]), cimag(data[i]));
    }

    free(data);
    return 0;
}