#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

vector<vector<int>> costMatrixBackup;

int reduceMatrix(vector<vector<int>>& costMatrix, int row, int col) {
    int reductionCost = 0;

    // Reduce the matrix
    for (int i = 0; i < costMatrix.size(); ++i) {
        if (i != row) {
            int minValue = INF;
            for (int j = 0; j < costMatrix[i].size(); ++j) {
                if (j != col && costMatrix[i][j] != INF) {
                    minValue = min(minValue, costMatrix[i][j]);
                }
            }
            if (minValue != INF) {
                reductionCost += minValue;
                for (int j = 0; j < costMatrix[i].size(); ++j) {
                    if (j != col && costMatrix[i][j] != INF) {
                        costMatrix[i][j] -= minValue;
                    }
                }
            }
        }
    }

    // Reduce the column
    for (int i = 0; i < costMatrix.size(); ++i) {
        if (i != col && costMatrix[i][col] != INF) {
            int minValue = *min_element(costMatrix[i].begin(), costMatrix[i].end());
            if (minValue != INF) {
                reductionCost += minValue;
                costMatrix[i][col] -= minValue;
            }
        }
    }

    return reductionCost;
}

int calculateBound(int bound, vector<vector<int>>& costMatrix) {
    // Calculate the bound
    for (int i = 0; i < costMatrix.size(); ++i) {
        int minValue = *min_element(costMatrix[i].begin(), costMatrix[i].end());
        if (minValue != INF) {
            bound += minValue;
        }
    }

    for (int j = 0; j < costMatrix.size(); ++j) {
        int minValue = INF;
        for (int i = 0; i < costMatrix.size(); ++i) {
            if (costMatrix[i][j] != INF) {
                minValue = min(minValue, costMatrix[i][j]);
            }
        }
        if (minValue != INF) {
            bound += minValue;
        }
    }

    return bound;
}

void tspBranchAndBound(vector<vector<int>>& costMatrix, vector<int>& path, int bound, int& minCost) {
    int n = costMatrix.size();
    int currentCity = path.back();

    if (path.size() == n) {
        // Complete path found
        int currentCost = bound + costMatrix[currentCity][path[0]];
        minCost = min(minCost, currentCost);

        // Print the path and cost
        cout << "Path: ";
        for (int city : path) {
            cout << city << " ";
        }
        cout << path[0] << endl;
        cout << "Cost: " << currentCost << endl;
        return;
    }

    for (int nextCity = 0; nextCity < n; ++nextCity) {
        if (find(path.begin(), path.end(), nextCity) == path.end()) {
            // Check if the edge is not infinity
            if (costMatrix[currentCity][nextCity] != INF) {
                int reducedCost = bound;
                costMatrixBackup = costMatrix;
                reducedCost += reduceMatrix(costMatrix, currentCity, nextCity);
                int newBound = calculateBound(reducedCost, costMatrix);

                if (newBound + costMatrix[nextCity][path[0]] < minCost) {
                    // Recursive call
                    path.push_back(nextCity);
                    tspBranchAndBound(costMatrix, path, newBound, minCost);

                    // Backtrack
                    path.pop_back();
                }

                // Restore the matrix
                costMatrix = costMatrixBackup;
            }
        }
    }
}

int main() {
    // Example cost matrix
    vector<vector<int>> costMatrix = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    int n = costMatrix.size();
    vector<int> path = {0};
    int bound = 0;
    int minCost = INF;

    tspBranchAndBound(costMatrix, path, bound, minCost);

    cout << "Minimum Cost: " << minCost << endl;

    return 0;
}
