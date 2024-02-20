#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <pthread.h>
using namespace std::chrono;


const int DIMENSION = 200;
#define THREAD_LIMIT 16
int matrixOne[DIMENSION][DIMENSION];
int matrixTwo[DIMENSION][DIMENSION];
int productMatrix[DIMENSION][DIMENSION];
int currentThreadCount = 0;
int slice = DIMENSION / THREAD_LIMIT;


void populateMatrixWithRandomValues(int targetMatrix[][DIMENSION]) {
    for (int row = 0; row < DIMENSION; ++row) {
        for (int column = 0; column < DIMENSION; ++column) {
            targetMatrix[row][column] = rand() % 100;
        }
    }
}


void* calculateMatrixProduct(void* args) {
    int threadPartition = currentThreadCount++;
    for (int i = threadPartition * slice; i < (threadPartition + 1) * slice; ++i) {
        for (int j = 0; j < DIMENSION; ++j) {
            productMatrix[i][j] = 0;
            for (int k = 0; k < DIMENSION; ++k) {
                productMatrix[i][j] += matrixOne[i][k] * matrixTwo[k][j];
            }
        }
    }
    return NULL;
}


int main() {
    srand(time(0));
    populateMatrixWithRandomValues(matrixOne);
    populateMatrixWithRandomValues(matrixTwo);
    auto startMoment = high_resolution_clock::now();
    pthread_t threads[THREAD_LIMIT];
    for (int i = 0; i < THREAD_LIMIT; ++i) {
        pthread_create(&threads[i], NULL, calculateMatrixProduct, NULL);
    }
    for (int i = 0; i < THREAD_LIMIT; ++i) {
        pthread_join(threads[i], NULL);
    }
    auto endMoment = high_resolution_clock::now();
    auto timeTaken = duration_cast<microseconds>(endMoment - startMoment);
    printf("Total computation time: %ld microseconds\n", timeTaken.count());


    std::ofstream resultFile("Threaded_Matrix_Product.txt");
    for (int row = 0; row < DIMENSION; ++row) {
        for (int column = 0; column < DIMENSION; ++column) {
            resultFile << productMatrix[row][column] << "\t";
        }
        resultFile << std::endl;
    }
    resultFile << "Total computation time: " << timeTaken.count() << " microseconds";
    resultFile.close();


    return 0;
}