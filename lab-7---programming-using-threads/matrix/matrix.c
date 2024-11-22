#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 20
#define NUM_THREADS 10

// Shared matrices
int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];

// Thread argument structure
typedef struct {
    int start_row;
    int end_row;
    int operation; // 0: Add, 1: Subtract, 2: Dot Product
} ThreadArgs;

// Function to initialize matrices
void initializeMatrices() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = i + j;
            matrixB[i][j] = i * j;
            resultMatrix[i][j] = 0;
        }
    }
}

// Parallel matrix addition
void* parallelMatrixAdd(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    
    for (int i = threadArgs->start_row; i < threadArgs->end_row; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            resultMatrix[i][j] = matrixA[i][j] + matrixB[i][j];
        }
    }
    
    return NULL;
}

// Parallel matrix subtraction
void* parallelMatrixSubtract(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    
    for (int i = threadArgs->start_row; i < threadArgs->end_row; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            resultMatrix[i][j] = matrixA[i][j] - matrixB[i][j];
        }
    }
    
    return NULL;
}

// Parallel matrix dot product
void* parallelMatrixDotProduct(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    
    for (int i = threadArgs->start_row; i < threadArgs->end_row; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int dot_product = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                dot_product += matrixA[i][k] * matrixB[k][j];
            }
            resultMatrix[i][j] = dot_product;
        }
    }
    
    return NULL;
}

// Print matrix
void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadArgs threadArgs[NUM_THREADS];
    
    // Initialize matrices
    initializeMatrices();
    
    // Divide work among threads (2 rows per thread for 20x20 matrix)
    for (int i = 0; i < NUM_THREADS; i++) {
        threadArgs[i].start_row = i * 2;
        threadArgs[i].end_row = (i + 1) * 2;
    }
    
    // Perform Matrix Addition
    printf("Matrix Addition:\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, parallelMatrixAdd, &threadArgs[i]);
    }
    
    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printMatrix(resultMatrix);
    
    // Perform Matrix Subtraction
    printf("\nMatrix Subtraction:\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, parallelMatrixSubtract, &threadArgs[i]);
    }
    
    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printMatrix(resultMatrix);
    
    // Perform Matrix Dot Product
    printf("\nMatrix Dot Product:\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, parallelMatrixDotProduct, &threadArgs[i]);
    }
    
    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printMatrix(resultMatrix);
    
    return 0;
}