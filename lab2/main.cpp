#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include "./include/medianFilter.h"

using namespace std;
using namespace std::chrono;

vector<vector<double>> loadPGM(const char* filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open file: " << filename << endl;
        exit(1);
    }

    string line;
    getline(file, line);
    if (line != "P5") {
        cerr << "Not a PGM file!" << endl;
        exit(1);
    }

    int width, height, maxval;
    file >> width >> height >> maxval;
    file.ignore();

    vector<vector<double>> img(height, vector<double>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            img[i][j] = file.get();
        }
    }

    return img;
}

void savePGM(const char* filename, const vector<vector<double>>& img) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open file for writing: " << filename << endl;
        exit(1);
    }

    file << "P5\n" << img[0].size() << " " << img.size() << "\n255\n";
    for (const auto& row : img) {
        for (const auto& pixel : row) {
            file.put(static_cast<unsigned char>(pixel));
        }
    }
}

int main() {
    const char* inputFilename = "image.pgm";
    const char* outputFilename = "../filteredImage.pgm";

    vector<vector<double>> img = loadPGM(inputFilename);

    int windowSize = 5;
    auto start = high_resolution_clock::now();
    vector<vector<double>> filteredMatrix = applyMedianFilterWithThreads(img, windowSize);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Median filter applied in " << duration.count() << " milliseconds." << endl;

    savePGM(outputFilename, filteredMatrix);

    cout << "Filtered image saved to " << outputFilename << endl;

    return 0;
}
