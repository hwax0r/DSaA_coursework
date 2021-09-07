//
// Created by David Sergeev on 07.09.2021.
//

#ifndef DSAA_COURSEWORK_TASK_H
#define DSAA_COURSEWORK_TASK_H

#include "vector"
#include "random"
#include "iostream"

#define NAME_OF(variable) ((decltype(&variable))nullptr, #variable)


class Task{
private:
    std::vector<std::vector<std::pair<std::string, int>>> work_cost_matrix;

public:

    std::string naming() {
        static int index = 1;
        int column_number = index;
        std::string result_string;
        while(column_number != 0){
            int rem = column_number % 26;
            if(rem==0){
                result_string = 'Z' + result_string;
                column_number = column_number / 26;
                column_number--;
            }
            else{
                result_string = char(rem + 64) + result_string;
                column_number = column_number / 26;
            }
        }
        index += 1;
        return result_string;
    }

    void output(const std::vector<std::vector<std::pair<std::string, int>>>& work_cost){
        std::cout << "Output of matrix: " << NAME_OF(work_cost) << "\n";
        for (const auto& worker:work_cost){
            std::cout << worker[0].first << ": ";
            for (const auto& cost:worker){
                std::cout << cost.second << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void output(const std::vector<std::vector<double>>& only_costs){
        std::cout << "Output of matrix: " << NAME_OF(only_costs) << "\n";
        for (const auto& vec:only_costs){
            for (const auto& cost:vec){
                std::cout << cost << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }



    void input_random(
            std::vector<std::vector<std::pair<std::string, int>>>& work_cost_matrix,
            std::vector<std::vector<double>>& only_costs){
        int matrix_size, rand_left_border, rand_right_border;
        while (true) {
            std::cout << "\n"
                      << "Вы выбрали случайное заполнение матрицы\n"
                      << "Введите размер матрицы, где в соответствии с заданием кол-во рабочих = кол-ву задач\n"
                      << "Размер матрицы ";
            std::cin >> matrix_size;
            std::cout << "\n"
                      << "Теперь нужно ввести границы стоимостей задач\n"
                      << "to generate cost for every task possible\n"
                      << "Левая граница: ";
            std::cin >> rand_left_border;
            std::cout << "Правая граница: ";
            std::cin >> rand_right_border;

            if (matrix_size > 0 && rand_right_border > 0 && rand_right_border < 30000
                && rand_right_border > rand_left_border) break;
            std::cout << "\n"
                      << "**********************************\n"
                      << "* Неверный ввод. Попробуй ещё.   *\n"
                      << "* размер матрицы > 0             *\n"
                      << "* правая граница > 0             *\n"
                      << "* левая граница < 30 000         *\n"
                      << "* правая граница > левая граница *\n"
                      << "**********************************\n\n";
        }
        work_cost_matrix.resize(matrix_size);
        only_costs.resize(matrix_size);

        std::random_device rd;
        std::mt19937_64 eng(rd());
        std::uniform_int_distribution<int> distr(rand_left_border, rand_right_border);

        for (int column = 0; column < matrix_size; column++) {
            for (int row = 0; row < matrix_size; row++) {

                int cost = distr(eng);

                if (column == 0) {
                    work_cost_matrix[row].resize(matrix_size);
                    only_costs[row].resize(matrix_size);

                    std::string worker_name = naming();

                    work_cost_matrix[row][column].first = worker_name;
                    work_cost_matrix[row][column].second = cost;

                    only_costs[row][column] = cost;
                } else {
                    work_cost_matrix[row][column].first = work_cost_matrix[row][column-1].first;
                    work_cost_matrix[row][column].second = cost;

                    only_costs[row][column] = cost;
                }
            }
        }
        output(work_cost_matrix);
    }

    void input_user(std::vector<std::vector<std::pair<std::string, int>>>& work_cost_matrix){
    }

//    void input(std::vector<std::vector<std::pair<std::string, int>>>& work_cost_matrix){
//        int choice;
//        while (true) {
//            std::cout << ""
//                      << "Choose input type\n"
//                      << "\t1 - random matrix of size = workers * (amount of works), where workers=(amount of works)\n"
//                      << "\t2 - user input \n"
//                      << "Your choice is: ";
//            std::cin >> choice;
//            if (choice == 1 or choice == 2) break;
//            std::cout << ""
//                      << "*********************************\n"
//                      << "*Wrong input. Try one more time.*\n"
//                      << "*choice must be 1 or 2          *\n"
//                      << "*********************************\n\n";
//        }
//
//        switch (choice) {
//            case 1:{
//                input_random(work_cost_matrix,);
//                break;
//            }
//            case 2: {
//                input_user(work_cost_matrix);
//                break;
//            }
//            default:
//                throw std::invalid_argument("L0L why wrong");
//        }
//    }

};

#endif //DSAA_COURSEWORK_TASK_H
