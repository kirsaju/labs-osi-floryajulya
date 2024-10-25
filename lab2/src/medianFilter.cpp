#include <iostream>
#include <algorithm>
#include <pthread.h>
#include "medianFilter.h"

using namespace std;

double getMedian(vector<double>& window) {
    int size = window.size();
    
    if (size % 2 == 1) {
        nth_element(window.begin(), window.begin() + size / 2, window.end());
        return window[size / 2];
    } 
    else {
        nth_element(window.begin(), window.begin() + size / 2, window.end());
        double a = window[size / 2];
        nth_element(window.begin(), window.begin() + size / 2 - 1, window.end());
        double b = window[size / 2 - 1];
        return (a + b) / 2.0;
    }
}

struct rowsMedianFilter_t {
    const vector<vector<double>>* matrix;
    vector<vector<double>>* filteredMatrix;
    int startRow;
    int step;
    int windowSize;
};

double getWindowMedian(const vector<vector<double>>& matrix, int i, int j, int windowSize) {
    int offset = windowSize / 2;

    vector<double> window;

    for (int m = i - offset; m <= i + offset; ++m) {
        for (int n = j - offset; n <= j + offset; ++n) {
            if (m >= 0 && m < (int)matrix.size() && n >= 0 && n < (int)matrix[0].size()) {
                window.push_back(matrix[m][n]);
            }
        }
    }

    return getMedian(window);
}

void *thread_rowsMedianFilter(void *params) {
    rowsMedianFilter_t *gwmParams = (rowsMedianFilter_t *)(params);
    for (int i = gwmParams->startRow; i < (int)gwmParams->matrix->size(); i += gwmParams->step) {
        for (int j = 0; j < (int)gwmParams->matrix->front().size(); j++) {
            (*(gwmParams->filteredMatrix))[i][j] = getWindowMedian(*(gwmParams->matrix), i, j, gwmParams->windowSize);
        }
    }
    delete gwmParams;
    pthread_exit(NULL);
}

vector<vector<double>> applyMedianFilterWithThreads(const vector<vector<double>>& matrix, int windowSize, int threadsCount) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    vector<vector<double>> filteredMatrix(rows, vector<double>(cols));
    vector<pthread_t> threads(threadsCount);

    for (int thread_id = 0; thread_id < threadsCount; thread_id++) {
        rowsMedianFilter_t *gwmParams = new rowsMedianFilter_t{&matrix, &filteredMatrix, thread_id, threadsCount, windowSize};
        pthread_create(&threads[thread_id], NULL, thread_rowsMedianFilter, (void*)gwmParams);
    }
 
    for (int thread_id = 0; thread_id < threadsCount; thread_id++) {
        pthread_join(threads[thread_id], NULL);
    }


    return filteredMatrix;
}

vector<vector<double>> applyMedianFilter(const vector<vector<double>>& matrix, int windowSize) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int offset = windowSize / 2;

    vector<vector<double>> filteredMatrix(rows, vector<double>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            vector<double> window;

            for (int m = i - offset; m <= i + offset; ++m) {
                for (int n = j - offset; n <= j + offset; ++n) {
                    if (m >= 0 && m < rows && n >= 0 && n < cols) {
                        window.push_back(matrix[m][n]);
                    }
                }
            }

            filteredMatrix[i][j] = getMedian(window);
        }
    }

    return filteredMatrix;
}

void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}
