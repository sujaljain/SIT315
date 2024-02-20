#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
using namespace std::chrono;


const int MATRIX_DIM = 200; // Changed SIZE to MATRIX_DIM for clarity


// Renamed generateRandomMatrix to fillMatrixWithRandomValues
void fillMatrixWithRandomValues(int matrix[][MATRIX_DIM]) {
    for (int row = 0; row < MATRIX_DIM; ++row) // Changed loop variables for clarity
        for (int col = 0; col < MATRIX_DIM; ++col) // Changed loop variables for clarity
            matrix[row][col] = rand() % 100;
}


// Renamed performMatrixMultiplication to calculateProductOfMatrices
void calculateProductOfMatrices(const int firstMatrix[][MATRIX_DIM], const int secondMatrix[][MATRIX_DIM], int productMatrix[][MATRIX_DIM]) {
    for (int row = 0; row < MATRIX_DIM; ++row) {
        for (int col = 0; col < MATRIX_DIM; ++col) {
            productMatrix[row][col] = 0;
            for (int k = 0; k < MATRIX_DIM; ++k) {
                productMatrix[row][col] += firstMatrix[row][k] * secondMatrix[k][col];
            }
        }
    }
}


int main() {
    srand(time(nullptr)); // Seed the random number generator


    int firstMatrix[MATRIX_DIM][MATRIX_DIM];
    int secondMatrix[MATRIX_DIM][MATRIX_DIM];
    int productMatrix[MATRIX_DIM][MATRIX_DIM];


    // Fill matrices with random numbers
    fillMatrixWithRandomValues(firstMatrix);
    fillMatrixWithRandomValues(secondMatrix);


    // Start timing the matrix multiplication
    auto startMoment = high_resolution_clock::now();


    // Multiply the matrices
    calculateProductOfMatrices(firstMatrix, secondMatrix, productMatrix);


    // Stop timing the matrix multiplication
    auto endMoment = high_resolution_clock::now();


    // Calculate the time taken for multiplication
    auto timeTaken = duration_cast<microseconds>(endMoment - startMoment);


    // Output the time taken to console
    std::cout << "Multiplication completed in: " << timeTaken.count() << " microseconds" << std::endl;


    // Write the result matrix and execution time to a file
    std::ofstream resultFile("Result_matrix.txt");
    for (int row = 0; row < MATRIX_DIM; ++row) {
        for (int col = 0; col < MATRIX_DIM; ++col) {
            resultFile << productMatrix[row][col] << "\t";
        }
        resultFile << std::endl;
    }
    resultFile << "Execution time: " << timeTaken.count() << " microseconds";
    resultFile.close();


    return 0;
}