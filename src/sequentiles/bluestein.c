#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>

// Cooley-Tukey FFT implementation for use in Bluestein's algorithm
void fft(complex double *X, int N, int inverse) {
    // Bit-reversal permutation
    for (int i = 0, j = 0; i < N; i++) {
        if (i < j) {
            complex double temp = X[i];
            X[i] = X[j];
            X[j] = temp;
        }
        for (int k = N >> 1; (j ^= k) < k; k >>= 1);
    }

    // FFT computation
    for (int len = 2; len <= N; len <<= 1) {
        double angle = 2 * M_PI / len * (inverse ? -1 : 1);
        complex double wlen = cos(angle) + I * sin(angle);
        for (int i = 0; i < N; i += len) {
            complex double w = 1;
            for (int j = 0; j < len / 2; j++) {
                complex double u = X[i + j];
                complex double t = w * X[i + j + len / 2];
                X[i + j] = u + t;
                X[i + j + len / 2] = u - t;
                w *= wlen;
            }
        }
    }

    // If inverse FFT, divide by the array size
    if (inverse) {
        for (int i = 0; i < N; i++) X[i] /= N;
    }
}

// Function to find the next power of two greater than or equal to N
int nextPowerOfTwo(int N) {
    return pow(2, ceil(log2(N)));
}

// Bluestein's FFT algorithm
void bluesteinsFFT(complex double *x, int N) {
    int M = nextPowerOfTwo(N * 2 - 1); // Length for zero-padding

    // Allocate memory for sequences
    complex double *a = calloc(M, sizeof(complex double));
    complex double *b = calloc(M, sizeof(complex double));

    // Precompute the chirp sequence and its inverse
    for (int k = 0; k < N; k++) {
        double angle = M_PI * k * k / N;
        complex double chirp = cexp(I * angle);
        a[k] = x[k] * chirp;
        b[k] = 1.0 / chirp; // Inverse chirp
    }

    // FFT of sequences
    fft(a, M, 0);
    fft(b, M, 0);

    // Convolution
    for (int k = 0; k < M; k++) {
        a[k] *= b[k];
    }

    // Inverse FFT
    fft(a, M, 1);

    // Multiply by the inverse chirp sequence
    for (int k = 0; k < N; k++) {
        x[k] = a[k] * b[k];
    }

    // Free allocated memory
    free(a);
    free(b);
}

// Main function
int main() {
    const int N = 5; // Length of the input sequence (can be non-power of 2)
    complex double x[N];

    // Example input
    for (int i = 0; i < N; i++) {
        x[i] = i + 1; // Arbitrary input data
    }

    // Perform FFT using Bluestein's algorithm
    bluesteinsFFT(x, N);

    // Output the results
    for (int i = 0; i < N; i++) {
        printf("x[%d] = %f + %fi\n", i, creal(x[i]), cimag(x[i]));
    }

    return 0;
}
