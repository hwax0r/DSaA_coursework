#include <iostream>
#include <vector>
#include <chrono>

#include "Task.h"
#include "Solution.h"
#include "hung_algo.h"




int main(){
    Task task;
    Solution solution;
    hungarian_algo solution_hung;
    std::vector<std::vector<std::pair<std::string, int>>> work_cost;
    std::vector<std::vector<double>> only_costs;

    task.input_random(work_cost, only_costs);

    auto start = std::chrono::steady_clock::now();
    solution.solve(work_cost);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto elapsed_milliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "My algo + graph gen time(mcs): " << elapsed_microseconds.count() << "\n";
    std::cout << "My algo + graph gen time(ms): " << elapsed_milliseconds.count() << "\n";
    std::cout << "My algo + graph gen time(sec): " << elapsed_seconds.count() << "\n";
    std::cout << "Graph nodes: " << solution.nodes_amount() << "\n\n";

    std::vector<int> assignment;

    auto start2 = std::chrono::steady_clock::now();
    double cost = solution_hung.solve(only_costs, assignment);
    std::cout << "Минимальная стоимость работ(Венгерский алгоритм): " << cost << "\n\n";
    auto end2 = std::chrono::steady_clock::now();
    auto elapsed_microseconds2 =
            std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    auto elapsed_milliseconds2 =
            std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
    auto elapsed_seconds2 =
            std::chrono::duration_cast<std::chrono::seconds>(end2 - start2);
    std::cout << "Hungarian algo time(mcs): " << elapsed_microseconds2.count() << "\n";
    std::cout << "Hungarian algo time(ms): " << elapsed_milliseconds2.count() << "\n";
    std::cout << "Hungarian algo time(sec): " << elapsed_seconds2.count() << "\n\n";

    task.output(work_cost);
    task.output(only_costs);

    return 0;
}