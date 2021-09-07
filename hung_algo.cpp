//
// Created by David Sergeev on 07.09.2021.
//

#include "hung_algo.h"

//
// Created by David Sergeev on 06.09.2021.
//

#include <cfloat> // for DBL_MAX
#include <cmath>  // for fabs()
#include "hung_algo.h"


hungarian_algo::hungarian_algo(){}
hungarian_algo::~hungarian_algo(){}


// Обёртка для выполлнения венгерского алгоритма
double hungarian_algo::solve(vector <vector<double> >& DistMatrix, vector<int>& assignment_in)
{
    unsigned int n_rows = DistMatrix.size();
    unsigned int n_cols = DistMatrix[0].size();

    double *distance_matrix_in = new double[n_rows * n_cols];
    int *assignment = new int[n_rows];
    double cost = 0.0;

    // Fill in the distance_matrix_in. Mind the index is "i + n_rows * j".
    // Here the cost matrix of size MxN is defined as a double precision array of N*M elements.
    // In the solving functions matrices are seen to be saved MATLAB-internally in row-order.
    // (i.e. the matrix [1 2; 3 4] will be stored as a vector [1 3 2 4], NOT [1 2 3 4]).
    for (unsigned int i = 0; i < n_rows; i++)
        for (unsigned int j = 0; j < n_cols; j++)
            distance_matrix_in[i + n_rows * j] = DistMatrix[i][j];

    // call solving function
    assignmentoptimal(assignment, &cost, distance_matrix_in, n_rows, n_cols);

    assignment_in.clear();
    for (unsigned int r = 0; r < n_rows; r++)
        assignment_in.push_back(assignment[r]);

    delete[] distance_matrix_in;
    delete[] assignment;
    return cost;
}


// Оптимальное решение для задачи назначения используя Венгерский алгоритм
void hungarian_algo::assignmentoptimal(int *assignment, double *cost, double *distMatrixIn, int nOfRows, int nOfColumns)
{
    double *distance_matrix, *distance_matrix_temp, *distance_matrix_end, *column_end, value, min_value;
    bool *covered_columns, *covered_rows, *star_matrix, *new_star_matrix, *prime_matrix;
    int elements_amount, minimum_dimension, row, col;

    // Инициализащия
    *cost = 0;
    for (row = 0; row<nOfRows; row++)
        assignment[row] = -1;

    // Создание копии матрицы дистанций
    // проверка элементов матрицы на наличие негативных элементов
    elements_amount = nOfRows * nOfColumns;
    distance_matrix = (double *)malloc(elements_amount * sizeof(double));
    distance_matrix_end = distance_matrix + elements_amount;

    for (row = 0; row < elements_amount; row++)
    {
        value = distMatrixIn[row];
        if (value < 0)
            cerr << "All matrix elements have to be non-negative." << endl;
        distance_matrix[row] = value;
    }


    // выделение памяти
    covered_columns = (bool *)calloc(nOfColumns, sizeof(bool));
    covered_rows = (bool *)calloc(nOfRows, sizeof(bool));
    star_matrix = (bool *)calloc(elements_amount, sizeof(bool));
    prime_matrix = (bool *)calloc(elements_amount, sizeof(bool));
    new_star_matrix = (bool *)calloc(elements_amount, sizeof(bool)); // используется в 4 шаге

    // Предварительные шаги
    if (nOfRows <= nOfColumns)
    {
        minimum_dimension = nOfRows;

        for (row = 0; row<nOfRows; row++)
        {
            // поиск минимального элемента в столбце
            distance_matrix_temp = distance_matrix + row;
            min_value = *distance_matrix_temp;
            distance_matrix_temp += nOfRows;
            while (distance_matrix_temp < distance_matrix_end)
            {
                value = *distance_matrix_temp;
                if (value < min_value)
                    min_value = value;
                distance_matrix_temp += nOfRows;
            }

            // вычитание минимального элемента из каждого элемента строки
            distance_matrix_temp = distance_matrix + row;
            while (distance_matrix_temp < distance_matrix_end)
            {
                *distance_matrix_temp -= min_value;
                distance_matrix_temp += nOfRows;
            }
        }

        // Шаг 1 и 2а
        for (row = 0; row<nOfRows; row++)
            for (col = 0; col<nOfColumns; col++)
                if (fabs(distance_matrix[row + nOfRows * col]) < DBL_EPSILON)
                    if (!covered_columns[col])
                    {
                        star_matrix[row + nOfRows * col] = true;
                        covered_columns[col] = true;
                        break;
                    }
    }
    else // if(nOfRows > nOfColumns) - если количество строк больше количества столбцов
    {
        minimum_dimension = nOfColumns;

        for (col = 0; col<nOfColumns; col++)
        {
            // поиск наименьшего элемента в столбце
            distance_matrix_temp = distance_matrix + nOfRows * col;
            column_end = distance_matrix_temp + nOfRows;

            min_value = *distance_matrix_temp++;
            while (distance_matrix_temp < column_end)
            {
                value = *distance_matrix_temp++;
                if (value < min_value)
                    min_value = value;
            }

            // вычитание минимального элемента из каждого элемента столбца
            distance_matrix_temp = distance_matrix + nOfRows * col;
            while (distance_matrix_temp < column_end)
                *distance_matrix_temp++ -= min_value;
        }

        // Шаг 1 и 2a
        for (col = 0; col<nOfColumns; col++)
            for (row = 0; row<nOfRows; row++)
                if (fabs(distance_matrix[row + nOfRows * col]) < DBL_EPSILON)
                    if (!covered_rows[row])
                    {
                        star_matrix[row + nOfRows * col] = true;
                        covered_columns[col] = true;
                        covered_rows[row] = true;
                        break;
                    }
        for (row = 0; row<nOfRows; row++)
            covered_rows[row] = false;

    }

    // Переход к шагу 2b
    step2b(assignment, distance_matrix, star_matrix, new_star_matrix, prime_matrix, covered_columns, covered_rows, nOfRows, nOfColumns, minimum_dimension);

    // вычисление стоимости и удаление ошибочных назначений
    compute_assignmentcost(assignment, cost, distMatrixIn, nOfRows);

    // очистка использованной памяти, выделенной под указатели
    free(distance_matrix);
    free(covered_columns);
    free(covered_rows);
    free(star_matrix);
    free(prime_matrix);
    free(new_star_matrix);

    return;
}

/********************************************************/
void hungarian_algo::build_assignment_vector(int *assignment, bool *starMatrix, int nOfRows, int nOfColumns)
{
    int row, col;

    for (row = 0; row<nOfRows; row++)
        for (col = 0; col<nOfColumns; col++)
            if (starMatrix[row + nOfRows*col])
            {
#ifdef ONE_INDEXING
                assignment[row] = col + 1; /* MATLAB-Indexing */
#else
                assignment[row] = col;
#endif
                break;
            }
}

/********************************************************/
void hungarian_algo::compute_assignmentcost(int *assignment, double *cost, double *distMatrix, int nOfRows)
{
    int row, col;

    for (row = 0; row<nOfRows; row++)
    {
        col = assignment[row];
        if (col >= 0)
            *cost += distMatrix[row + nOfRows*col];
    }
}

/********************************************************/
void hungarian_algo::step2a(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim)
{
    bool *starMatrixTemp, *columnEnd;
    int col;

    // покрытие каждого столбца, содержащего значащий ноль
    for (col = 0; col<nOfColumns; col++)
    {
        starMatrixTemp = starMatrix + nOfRows*col;
        columnEnd = starMatrixTemp + nOfRows;
        while (starMatrixTemp < columnEnd){
            if (*starMatrixTemp++)
            {
                coveredColumns[col] = true;
                break;
            }
        }
    }

    // переход к шагу 3
    step2b(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
}

/********************************************************/
void hungarian_algo::step2b(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim)
{
    int col, nOfCoveredColumns;

    // подсчёт покрытых столбцов
    nOfCoveredColumns = 0;
    for (col = 0; col<nOfColumns; col++)
        if (coveredColumns[col])
            nOfCoveredColumns++;

    if (nOfCoveredColumns == minDim)
    {
        // алгоритм завершён
        build_assignment_vector(assignment, starMatrix, nOfRows, nOfColumns);
    }
    else
    {
        // перезод к шагу 3
        step3(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
    }

}

/********************************************************/
void hungarian_algo::step3(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim)
{
    bool zerosFound;
    int row, col, starCol;

    zerosFound = true;
    while (zerosFound)
    {
        zerosFound = false;
        for (col = 0; col<nOfColumns; col++)
            if (!coveredColumns[col])
                for (row = 0; row<nOfRows; row++)
                    if ((!coveredRows[row]) && (fabs(distMatrix[row + nOfRows*col]) < DBL_EPSILON))
                    {
                        // значащий ноль
                        primeMatrix[row + nOfRows*col] = true;

                        // поиск значащего нуля в текущей строке
                        for (starCol = 0; starCol<nOfColumns; starCol++)
                            if (starMatrix[row + nOfRows*starCol])
                                break;

                        if (starCol == nOfColumns) // нет значащих нулей
                        {
                            // переход к шагу 4
                            step4(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim, row, col);
                            return;
                        }
                        else
                        {
                            coveredRows[row] = true;
                            coveredColumns[starCol] = false;
                            zerosFound = true;
                            break;
                        }
                    }
    }

    // переход к шагу 5
    step5(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
}

/********************************************************/
void hungarian_algo::step4(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim, int row, int col)
{
    int n, starRow, starCol, primeRow, primeCol;
    int nOfElements = nOfRows*nOfColumns;

    // временная копия starMatrix
    for (n = 0; n<nOfElements; n++)
        newStarMatrix[n] = starMatrix[n];

    // отметить текущий ноль
    newStarMatrix[row + nOfRows*col] = true;

    // отметить основной ноль в текущем столбце
    starCol = col;
    for (starRow = 0; starRow<nOfRows; starRow++)
        if (starMatrix[starRow + nOfRows*starCol])
            break;

    while (starRow<nOfRows)
    {
        /* unstar the starred zero */
        // убрать отметку с отмеченного нуля
        newStarMatrix[starRow + nOfRows*starCol] = false;

        /* find primed zero in current row */
        primeRow = starRow;
        for (primeCol = 0; primeCol<nOfColumns; primeCol++)
            if (primeMatrix[primeRow + nOfRows*primeCol])
                break;

        /* star the primed zero */
        // отметить основной ноль
        newStarMatrix[primeRow + nOfRows*primeCol] = true;

        /* find starred zero in current column */
        starCol = primeCol;
        for (starRow = 0; starRow<nOfRows; starRow++)
            if (starMatrix[starRow + nOfRows*starCol])
                break;
    }

    /* use temporary copy as new starMatrix */
    /* delete all primes, uncover all rows */

    for (n = 0; n<nOfElements; n++)
    {
        primeMatrix[n] = false;
        starMatrix[n] = newStarMatrix[n];
    }
    for (n = 0; n<nOfRows; n++)
        coveredRows[n] = false;

    // переход к шагу 2а
    step2a(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
}

/********************************************************/
void hungarian_algo::step5(int *assignment, double *distMatrix, bool *starMatrix, bool *newStarMatrix, bool *primeMatrix, bool *coveredColumns, bool *coveredRows, int nOfRows, int nOfColumns, int minDim)
{
    double h, value;
    int row, col;

    // поиск наименьшего непокрытого h
    h = DBL_MAX;
    for (row = 0; row<nOfRows; row++)
        if (!coveredRows[row])
            for (col = 0; col<nOfColumns; col++)
                if (!coveredColumns[col])
                {
                    value = distMatrix[row + nOfRows*col];
                    if (value < h)
                        h = value;
                }

    // добавление h к каждому покрытому столбцу
    for (row = 0; row<nOfRows; row++)
        if (coveredRows[row])
            for (col = 0; col<nOfColumns; col++)
                distMatrix[row + nOfRows*col] += h;

    /* subtract h from each uncovered column */
    // вычитание h из непокрытых столбцов
    for (col = 0; col<nOfColumns; col++)
        if (!coveredColumns[col])
            for (row = 0; row<nOfRows; row++)
                distMatrix[row + nOfRows*col] -= h;

    // переход к шагу 3
    step3(assignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);
}