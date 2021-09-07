//
// Created by David Sergeev on 07.09.2021.
//

#ifndef DSAA_COURSEWORK_HUNG_ALGO_H
#define DSAA_COURSEWORK_HUNG_ALGO_H

#include <iostream>
#include <vector>

using namespace std;


class hungarian_algo
{
public:
    hungarian_algo();
    ~hungarian_algo();
    double solve(vector <vector<double> >& DistMatrix, vector<int>& assignment_in);

private:
    void assignmentoptimal(int *assignment, double *cost, double *distMatrix, int nOfRows, int nOfColumns);
    void build_assignment_vector(int *assignment, bool *starMatrix, int nOfRows, int nOfColumns);
    void compute_assignmentcost(int *assignment, double *cost, double *distMatrix, int nOfRows);
    void step2a(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim);
    void step2b(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim);
    void step3(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim);
    void step4(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim, int row, int col);
    void step5(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim);
};


#endif //DSAA_COURSEWORK_HUNG_ALGO_H
