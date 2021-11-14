#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <iostream>
using namespace std;

int  main() {
    float avgSerial[5];
    float avgParallel1[5];
    float avgParallel2[5];
    clock_t start, end;
    int rowA, colA, rowB, colB, rowC, colC = 0;
    FILE* fpA = fopen("matrixA2500.txt", "r");
    FILE* fpB = fopen("matrixB2500.txt", "r");
    FILE* fpC = fopen("matrixC.txt", "w");

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
        return 0;
    }
    //Memory space unabailavale
    //add code later

    //Matrix C dimmensions;
    rowC = rowA;
    colC = colB;

    //Matrix A data input
    //Matrix A memory allocation
    double** matA;  
    matA = (double**)malloc(rowA * sizeof(double*));
    for (int i = 0; i < rowA; i++) {
        matA[i] = (double*)malloc(colA * sizeof(double));
    }
    if (NULL != fpA) {
        printf("Reading MAtrix A\n");
        
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
    if (NULL != fpA) {
        for (int c = 0; c < colB; c++) {
            for (int r = 0; r < rowB; r++) {
                double temp;
                fscanf(fpB, "%lf", &temp);
                matB[r][c] = temp;
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
        avgSerial[s] = end - start;
    }

    //Matrix C output to file
    for (int r = 0; r < rowC; r++) {
        for (int c = 0; c < colC; c++) {
            fprintf(fpC, "%20.10lf\n", matC[r][c]);
        }
    }


    //loop metodo de paralelizacion 1
    /*
    for (int s = 0; s < 5; s++) {
        start = clock();


        end = clock();
        avgParallel1[s] = end - start;
    }
    */
    //loop metodo de paralelizacion 2
    /*
    for (int s = 0; s < 5; s++) {
        start = clock();


        end = clock();
        avgParallel2[s] = end - start;
    }
    */
    
    //memory freeing
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
    
    fclose(fpA);
    fclose(fpB);
    fclose(fpC);

    return 0;
}
