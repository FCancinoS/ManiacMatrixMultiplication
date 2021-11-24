#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <iostream>
#include <omp.h>
#include <math.h>
#include <intrin.h>
#define ompthreads 4
using namespace std;

int  main() {
    printf("Program starting\n");
    double avgSerial = 0; 
    double avgParallel1 = 0;
    double avgParallel2 = 0;
    double tSerial[5];
    double tParallel1[5];
    double tParallel2[5];
    double frequency = 2.6;
    double dummy;
    int rCount = 0;
    frequency = 2.6;
    int  errorcount2 = 0;
    int errorcount1 = 0;
    clock_t start, end;
    int rowA, colA, rowB, colB, rowC, colC = 0;
    FILE* fpA = fopen("matrixA.txt", "r");
    FILE* fpB = fopen("matrixB.txt", "r");
    FILE* fpC = fopen("matrixC.txt", "w");
    int dimA, dimB, dimC;
    
    //matrix A dimensions
    printf("Enter the number of rows for Maxtrix A:\n");
    cin >> rowA;
    printf("Enter the number of collumns for Maxtrix A:\n");
    cin >> colA;

    //matrix B dimensions
    printf("Enter the number of rows for Maxtrix B:\n");
    cin >> rowB;
    printf("Enter the number of collumns for Maxtrix B:\n");
    cin >> colB;

    //dimension error checking
    //Multiplication isn't posible
    if (rowB != colA) {
        printf("Unabale to perform operation due to input matrix dimensions\n");
        fclose(fpA);
        fclose(fpB);
        fclose(fpC);
        return 0;
    }
    

    //Matrix C dimmensions;
    rowC = rowA;
    colC = colB;

    dimA = rowA * colA;
    dimB = rowB * colB;
    dimC = rowC * colC;

    //Matrix dimension size checking
    while (fscanf(fpA, "%lf", &dummy) != EOF) {
        rCount++;
    }
    if (rCount < dimA) {
        fclose(fpA);
        fclose(fpB);
        fclose(fpC);
        printf("Error, Matrix A dimensions are too big for the file contents\n");
        return 0;
    }
    rCount = 0;
    while (fscanf(fpB, "%lf", &dummy) != EOF) {
        rCount++;
    }

    if (rCount < dimB) {
        fclose(fpA);
        fclose(fpB);
        fclose(fpC);
        printf("Error, Matrix B dimensions are too big for the file contents\n");
        return 0;
    }

    fclose(fpA);
    fclose(fpB);

    fpA = fopen("matrixA.txt", "r");
    fpB = fopen("matrixB.txt", "r");


    //Matrix A data input
    //Matrix A memory allocation
    double** matA;  
    matA = (double**)malloc(rowA * sizeof(double*));
    for (int i = 0; i < rowA; i++) {
        matA[i] = (double*)malloc(colA * sizeof(double));
    }
    //Memory space unabailavale
    if (**matA == NULL) {
        fclose(fpA);
        fclose(fpB);
        fclose(fpC);
        for (int i = 0; i < rowA; i++) {
            free(matA[i]);
        }
        free(matA);
        printf("Error, Matrix A dimensions too big for memory space");
        return 0;
    }
    else if (NULL != fpA) {
            for (int r = 0; r < rowA; r++) {
                for (int c = 0; c < colA; c++) {
                    double temp;
                    fscanf(fpA, "%lf", &temp);
                    matA[r][c] = temp;
                }
            }      
    }
    else {
        printf("Matrix A file not found");
        return 0;
    }
    
    //Matrix B data input
    //Matrix B memory allocation
    double** matB;
    matB = (double**)malloc(colB * sizeof(double*));
    for (int i = 0; i < colB; i++) {
        matB[i] = (double*)malloc(rowB * sizeof(double));
    }
    if (*matB == NULL) {
        fclose(fpA);
        fclose(fpB);
        fclose(fpC);
        for (int i = 0; i < rowA; i++) {
            free(matA[i]);
        }
        free(matB);
        for (int i = 0; i < colB; i++) {
            free(matB[i]);
        }
        free(matB);
        printf("Error, Matrix B dimensions too big for memory space");
        return 0;
    }
    else if (NULL != fpB) {
        for (int r = 0; r < rowB; r++) {
            for (int c = 0; c < colB; c++) {
                double temp;
                fscanf(fpB, "%lf", &temp);
                matB[c][r] = temp;
            }
        }
    }
    else {
        printf("Matrix B file not found");
        return 0;
    }
    

    //Matrix C memory allocation
    double** matC;
    matC = (double**)malloc(rowC * sizeof(double*));
    for (int i = 0; i < rowC; i++) {
        matC[i] = (double*)malloc(colC * sizeof(double));
    }
    //Matrix C1 memory allocation
    double** matC1;
    matC1 = (double**)malloc(rowC * sizeof(double*));
    for (int i = 0; i < rowC; i++) {
        matC1[i] = (double*)malloc(colC * sizeof(double));
    }
    //Matrix C2 memory allocation
    double** matC2;
    matC2 = (double**)malloc(rowC * sizeof(double*));
    for (int i = 0; i < rowC; i++) {
        matC2[i] = (double*)malloc(colC * sizeof(double));
    }

    printf("\n\nEntrando a serial \n\n");
    //loop serial
    for (int s = 0; s < 5; s++) {
        start = clock();
        for (int r = 0; r < rowC; r++) {
            for (int c = 0; c < colC; c++) {
                double acum = 0;
                for (int i = 0; i < colA; i++) {
                    acum += matA[r][i] * matB[c][i];
                }
                matC[r][c] = acum;
            }
        }
        end = clock();
        tSerial[s] = (end - start) / frequency;
    }
    printf("\n\nSaliendo de serial \n\n");
    //Matrix C output to file
    for (int r = 0; r < rowC; r++) {
        for (int c = 0; c < colC; c++) {
            fprintf(fpC, "%.10lf\n", matC[r][c]);
        }
    }


    //loop metodo de paralelizacion 1
    printf("\n\nEntrando a OMP \n\n");
    omp_set_num_threads(ompthreads);
    double* pAcum;
    pAcum = (double*)malloc(ompthreads * sizeof(double));

    for (int s = 0; s < 5; s++) {
        start = clock();
        #pragma omp parallel 
        {
            int numThread = omp_get_thread_num();
            for (int r = numThread; r < rowC; r += ompthreads) {
                for (int c = 0; c < colC; c++) {
                    pAcum[numThread] = 0;
                    for (int i = 0; i < colA; i++) {
                        pAcum[numThread] += matA[r][i] * matB[c][i];
                    }
                    matC1[r][c] = pAcum[numThread];
                }
            }
        }
        end = clock();
        tParallel1[s] = (end - start)  / frequency;
    }
    printf("\n\nSaliendo de OMP \n\n");

    //comparar metodo 1:
    for (int r = 0; r < rowC; r++) {
        for (int c = 0; c < colC; c++) {
            if (matC[r][c] != matC1[r][c]) {
                errorcount1++;
            }
        }
    }
    if (errorcount1 > 0) {
        printf("OMP parallelization output does not match serial algorithm output\n");
    }
    printf("Parallelization method 1: OMP error count: %d\n\n", errorcount1);


    //loop metodo de paralelizacion 2
    double* matrizA = (double*)malloc(rowA * colA * sizeof(double));
    double* matrizB = (double*)malloc(rowB * colB * sizeof(double));
    double* matrizC = (double*)malloc(rowC * colC * sizeof(double));
    printf("\n\nEntrando a intrinsics \n\n");
    for (int r = 0; r < colA; r++) {
        for (int c = 0; c < rowA; c++) {
            double temp;
            fscanf(fpA, "%lf", &temp);
            matrizA[r * rowA + c] = temp;
        }
    }

    for (int c = 0; c < colB; c++) {
        for (int r = 0; r < rowB; r++) {
            double temp;
            fscanf(fpB, "%lf", &temp);
            matrizB[r * colB + c] = temp;
        }
    }
    printf("\n\nRelleno de matrices completado \n\n");
    for (int s = 0; s < 5; s++) {
        start = clock();
        /*multiplicaIntrinsics(matrizC, matrizA, matrizB, colC, rowC, colA, rowA, colB, rowB);*/
        for (int i = 0; i < colC; i++) {
            for (int j = 0; j < rowC; j++) {
                matrizC[i * rowC + j] = 0;
                __m256d a_aux;
                __m256d b_aux;
                __m256d c_aux;
                double* section;
                section = (double*)malloc(sizeof(double) * 4);
                for (int k = 0; k < 4; k++) {
                    section[k] = 0;
                }
                int relleno = rowA % 4;
                for (int l = 0; l < rowA - relleno; l += 4) {
                    a_aux = _mm256_load_pd(&matrizA[i * rowA + l]);
                    b_aux = _mm256_load_pd(&matrizB[i * rowA + l]);
                    c_aux = _mm256_mul_pd(a_aux, b_aux);
                    _mm256_store_pd(section, c_aux);
                }
                for (int m = rowA - relleno; m < rowA; m++) {
                    matrizC[i * rowC + j] += matrizA[i * rowA + m] * matrizB[j * rowA + m];
                }
                free(section);
            }
        }
        end = clock();
        tParallel2[s] = (end - start) / frequency;
    }
    printf("\n\nSaliendo de intrinsics \n\n");
    
    //comparar metodo 2:
    fclose(fpC);
    fpC = fopen("matrixC.txt", "r");
    double* matrizCompare = (double*)malloc(rowC * colC * sizeof(double));
    for (int r = 0; r < colC; r++) {
        for (int c = 0; c < rowC; c++) {
            double temp;
            fscanf(fpC, "%lf", &temp);
            matrizCompare[r * rowC + c] = temp;
        }
    }
    for (int i = 0; i < colC; i++) {
        for (int j = 0; j < rowC; j++) {
            int relleno = rowA % 4;
            for (int m = rowA - relleno; m < rowA; m++) {
                if (matrizC[i * rowC + j] != matrizCompare[i * rowC + j]) {
                    errorcount2++;
               }
            }
        }
    }
    if (errorcount2 > 0) {
        printf("intrinsics parallelization output does not match serial algorithm output\n");
    }
    printf("Parallelization method 2: intrinsics error count: %d\n\n", errorcount2);

    //time averages
    for (int i = 0; i < 5; i++) {
        avgSerial += tSerial[i];
        avgParallel1 += tParallel1[i];
        avgParallel2 += tParallel2[i];
    }
    avgSerial = avgSerial / 5;
    avgParallel1 = avgParallel1 / 5;
    avgParallel2 = avgParallel2 / 5;

    //print table
    printf("Run     Serial     Parallel 1     Parallel 2\n");
    for (int i = 0; i < 5; i++) {
        printf(" %d        %3.lf        %.3lf        %.3lf\n", i, tSerial[i], tParallel1[i], tParallel2[i]);
    }
    printf("AVG     %.3lf      %.3lf      %.3lf\n", avgSerial, avgParallel1, avgParallel2);
    printf("speedup     -       %.3lf        %.3lf\n", (avgParallel1 / avgSerial)*100, (avgParallel2 / avgSerial)*100);
   
    if (avgParallel1 < avgParallel2) {
        printf("Parallelization method 1: OMP is faster");
    }
    else {
        printf("Parallelization method 2: intrinsics is faster");
    }


    //memory freeing
    printf("\n\nLiberando memoria \n\n");

    free(matrizA);
    free(matrizB);
    free(matrizC);
    for (int i = 0; i < rowA; i++) {
        free(matA[i]);
    }
    free(matA);
 
    for (int i = 0; i < colB; i++) {
        free(matB[i]);
    }
    free(matB);

    for (int i = 0; i < rowA; i++) {
        free(matC[i]);
    }
    free(matC);

    for (int i = 0; i < rowA; i++) {
        free(matC1[i]);
    }
    free(matC1);
    
    for (int i = 0; i < rowA; i++) {
        free(matC2[i]);
    }
    free(matC2);

    fclose(fpA);
    fclose(fpB);
    fclose(fpC);

    return 0;
}
