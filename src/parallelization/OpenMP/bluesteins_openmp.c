#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <omp.h>

#define PI 3.14159265358979323846

// Utility function for complex exponential
complex double cexpd(double x) {
    return cos(x) + I * sin(x);
}

// Naive DFT (replace with an efficient FFT for a complete implementation)
void fft(complex double *X, int N, complex double *x) {
    for (int k = 0; k < N; ++k) {
        X[k] = 0;
        for (int n = 0; n < N; ++n) {
            double theta = (2 * PI * k * n) / N;
            X[k] += x[n] * cexpd(-theta);
        }
    }
}

// Bluestein's FFT algorithm
void bluestein_fft(complex double *x, int N) {
    // Find an M that is a power of 2 and >= 2*N-1
    int M = 1;
    while (M < 2 * N - 1) {
        M *= 2;
    }

    // Allocate arrays
    complex double *a = malloc(M * sizeof(complex double));
    complex double *b = malloc(M * sizeof(complex double));
    complex double *A = malloc(M * sizeof(complex double));
    complex double *B = malloc(M * sizeof(complex double));

    // Initialize 'a' and 'b'
    #pragma omp parallel for
    for (int n = 0; n < N; ++n) {
        a[n] = x[n] * cexpd(-PI * n * n / N);
        b[n] = cexpd(PI * n * n / N);
    }
    for (int n = N; n < M; ++n) {
        a[n] = 0;
        b[n] = 0;
    }

    // Compute FFT of 'a' and 'b'
    fft(A, M, a);
    fft(B, M, b);

    // Convolution: Multiply the results
    #pragma omp parallel for
    for (int k = 0; k < M; ++k) {
        A[k] *= B[k];
    }

    // Inverse FFT of A
    fft(a, M, A);

    // Multiply by the chirp again
    #pragma omp parallel for
    for (int n = 0; n < N; ++n) {
        x[n] = a[n] * cexpd(-PI * n * n / N) / M;
    }

    // Free allocated memory
    free(a); free(b); free(A); free(B);
}

int main() {
    int N = 8; // Example size
    complex double *x = malloc(N * sizeof(complex double));

    // Initialize x with your data
    for (int i = 0; i < N; ++i) {
        x[i] = i;
    }

    // Compute FFT
    bluestein_fft(x, N);

    // Output results
    for (int i = 0; i < N; ++i) {
        printf("%f + %fi\n", creal(x[i]), cimag(x[i]));
    }

    free(x);
    return 0;
}
