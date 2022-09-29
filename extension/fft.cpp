#include <bits/stdc++.h>

int x[1000][1000];

void FFT(int N, std::complex<double> *x) {
    /* bit-reversal permutation */
    for (int i = 1, j = 0; i < N; ++i) {
        for (int k = N >> 1; !((j ^= k) & k); k >>= 1)
            ;
        if (i > j)
            swap(x[i], x[j]);
    }

    /* dynamic programming */
    for (int k = 2; k <= N; k <<= 1) {
        float theta = -2.0 * 3.14159 / k;  //float theta = 2.0 * 3.14159 / k; (逆變換)
        std::complex<float> delta_w(cos(theta), sin(theta));

        // 每k個做一次FFT
        for (int j = 0; j < N; j += k) {
            // 前k/2個與後k/2的三角函數值恰好對稱，
            // 因此兩兩對稱的一起做。
            std::complex<double> w(1, 0);
            for (int i = j; i < j + k / 2; i++) {
                std::complex<double> a = x[i];
                std::complex<double> b = x[i + k / 2] * w;
                x[i] = a + b;
                x[i + k / 2] = a - b;
                w *= delta_w;
            }
        }
    }
    //scale
    for (int i = 0; i < N; i++) {
        x[i] /= sqrt(N);
    }
}

// Varables Info
//###########################################
int M = 1000;
int N = 1000;
int InputImage[1000][1000];
double FreqReal[1000][1000];
double FreqImag[1000][1000];

//each col
std::complex<double> **temp = new std::complex<double> *[M];

for (int i = 0; i < M; i++) {
    temp[i] = new std::complex<double>[N];
}

for (int i = 0; i < M; i++) {
    //將一維的資料抽出來
    std::complex<double> *x = new std::complex<double>[N];
    for (int j = 0; j < N; j++) {
        //資料來源用原圖
        x[j].real(InputImage[i][j]);
        x[j].imag(0);
    }
    FFT(N, x);
    for (int j = 0; j < N; j++) {
        //將資料暫存到一個空間
        temp[i][j] = x[j];
    }
    delete[] x;
}
//each row
for (int i = 0; i < N; i++) {
    //將一維的資料抽出來
    std::complex<double> *x = new std::complex<double>[M];
    for (int j = 0; j < M; j++) {
        //資料來源用剛剛的結果
        x[j] = temp[j][i];
    }
    FFT(M, x);
    for (int j = 0; j < M; j++) {
        FreqReal[i][j] = x[j].real();
        FreqImag[i][j] = x[j].imag();
    }
    delete[] x;
}

//實數虛數結合
for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
        OutputImage[i][j] = sqrt(pow(FreqReal[i][j], 2) + pow(FreqImag[i][j], 2));
    }
}