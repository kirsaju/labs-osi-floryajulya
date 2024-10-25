#include <vector>

using namespace std;

vector<vector<double>> applyMedianFilterWithThreads(const vector<vector<double>>& matrix, int windowSize, int threadsCount = 8);
vector<vector<double>> applyMedianFilter(const vector<vector<double>>& matrix, int windowSize);