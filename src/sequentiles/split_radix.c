#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdbool.h>

// Function to check if a number is a power of two
bool isPowerOfTwo(int n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

// Function to compute the bit reversal of an integer
unsigned int bitReverse(unsigned int x, int log2n) {
    int n = 0;
    for (int i = 0; i < log2n; i++) {
        n <<= 1;
        n |= (x & 1);
        x >>= 1;
    }
    return n;
}

// Split-Radix FFT algorithm
int splitRadixFFT(complex double *X, int N) {
    if (X == NULL) {
        fprintf(stderr, "Error: Input array is null.\n");
        return -1;
    }

    if (!isPowerOfTwo(N)) {
        fprintf(stderr, "Error: Array size is not a power of 2.\n");
        return -1;
    }

    int log2N = log2(N);

    // Bit reversal of the given array
    for (unsigned int i = 0; i < N; ++i) {
        unsigned int rev = bitReverse(i, log2N);
        if (i < rev) {
            complex double temp = X[i];
            X[i] = X[rev];
            X[rev] = temp;
        }
    }

    // Main FFT computation using Split-Radix
    for (int len = 2; len <= N; len <<= 1) {
        double angle = -2 * M_PI / len;
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

    return 0; // Success
}

// Main function
int main() {
    const int N = 8; // Size of the array (must be a power of 2)
    complex double X[N];

    // Example input
    for (int i = 0; i < N; i++) {
        X[i] = i + I * i;
    }

    // Perform FFT using Split-Radix
    if (splitRadixFFT(X, N) != 0) {
        return 1; // Error occurred
    }

    // Output the results
    for (int i = 0; i < N; i++) {
        printf("X[%d] = %f + %fi\n", i, creal(X[i]), cimag(X[i]));
    }

    return 0;
}
