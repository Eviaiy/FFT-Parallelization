#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdbool.h>

// Check if a number is a power of two
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

// Cooley-Tukey FFT algorithm
int cooleyTukeyFFT(complex double *X, int N) {
    // Check for null pointer
    if (X == NULL) {
        fprintf(stderr, "Error: Input array is null.\n");
        return -1;
    }

    // Check if N is a power of 2
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

    // FFT computation
    for (int s = 1; s <= log2N; ++s) {
        int m = 1 << s; // 2 power s
        complex double wm = cexp(-2.0 * M_PI * I / m); // Twiddle factor

        for (int k = 0; k < N; k += m) {
            complex double w = 1.0;
            for (int j = 0; j < m / 2; ++j) {
                complex double t = w * X[k + j + m / 2];
                complex double u = X[k + j];
                X[k + j] = u + t;
                X[k + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }

    return 0; // Success
}

// Main function
int main() {
    const int N = 8; // Size of the array (must be a power of 2)
    complex double X[N];

    // Example input (could be any complex numbers)
    for (int i = 0; i < N; i++) {
        X[i] = i + I * i;
    }

    // Perform FFT
    if (cooleyTukeyFFT(X, N) != 0) {
        return 1; // Error occurred
    }

    // Output the results
    for (int i = 0; i < N; i++) {
        printf("X[%d] = %f + %fi\n", i, creal(X[i]), cimag(X[i]));
    }

    return 0;
}