//
// Created by David Sergeev on 07.09.2021.
//

#ifndef DSAA_COURSEWORK_SOLUTION_H
#define DSAA_COURSEWORK_SOLUTION_H

#include "GraphNode.h"
#include "iostream"
#include "stack"

class Solution{
private:
    Graph_node head;
    Graph_node tail;
    std::vector<std::vector<std::pair<std::string, int>>> work_cost;
    std::vector<std::vector<std::pair<std::string, int>>> work_cost_transposed;
    std::vector<std::map<std::string, int>> levels;
    std::vector<Graph_node> endings;
    int node_counter = 2;

public:
    Solution()= default;

    void solve(const std::vector<std::vector<std::pair<std::string, int>>>& work_cost_matrix){
        this->work_cost = work_cost_matrix;
        levels.resize(work_cost.size());

        transpose();
        gen_levels();

        head.name = "head";
        head.worker_cost = 0;
        head.level = -1;
        head.possibilities = levels[0];

        graph_generator();

        std::sort(endings.begin(), endings.end(), output_cmp);
        int minimum_overall_cost = endings[0].cost_to_this_node;
        std::cout << "Минимальная стоимость работ: " << minimum_overall_cost << "\n";
        std::cout << "Возможные варинты назначения: \n";
        for (const Graph_node& node:endings){
            if (node.cost_to_this_node != minimum_overall_cost)
                break;
            for (const std::string& worker:node.route){
                std::cout << worker << " ";
            }
            std::cout << "\n";
        }
        std::cout << "Задача выполнена.\n";
    }


    void transpose(){
        // результирующая матрица
        std::vector<std::vector<std::pair<std::string, int>>> transposed(work_cost.size());
        // транспонирование
        for (int col = 0; col < work_cost.size(); col++){
            transposed[col].resize(work_cost.size());
            int el = 0;
            for (const auto& vec : work_cost){
                transposed[col][el] = vec[col] ;
                el++;
            }
        }
        work_cost_transposed = transposed;
    }

    void gen_levels(){
        for (int level = 0; level < levels.size(); level++){
            std::map<std::string, int> temp(work_cost_transposed[level].begin(), work_cost_transposed[level].end());
            levels[level] = temp;
        }
    }

    void graph_generator(){

        std::stack<Graph_node*> stack_;
        stack_.push(&head);
        while (!stack_.empty()){
            Graph_node* processing_node = stack_.top();
            stack_.pop();
            // обработка ноды, пока у неё есть новые пути
            while (!processing_node->possibilities.empty()){

                // инициализация новой ноды графа
                std::string name = processing_node->possibilities.begin()->first; // имя новой ноды
                int cost_to_this = processing_node->possibilities.begin()->second; // вес новой ноды
                // новая нода
                Graph_node node(name, cost_to_this, processing_node->level+1);
                node.keys_to_be_deleted = processing_node->keys_to_be_deleted;
                node.keys_to_be_deleted.push_back(name);
                node.cost_to_this_node = processing_node->cost_to_this_node + node.worker_cost;
                node.route = processing_node->route;
                node.route.push_back(node.name);
                node_counter++;
                // смена уровня(набора стоимостей выполнения задач) для новой ноды
                std::map<std::string, int> new_possibilities = {};
                if (node.level + 1 < levels.size()){
                    new_possibilities = levels[node.level + 1];
                }

                // удаление пройденных нод из списка возможных путей
                for (const std::string& key: node.keys_to_be_deleted){
                    if (new_possibilities.find(key) != new_possibilities.end()){
                        new_possibilities.erase(key);
                    }
                }
                node.possibilities = new_possibilities;

                // в обрабатываемой ноде удаляем уже созданный путь
                processing_node->possibilities.erase(name);
                // в обрабатываемую ноду добавляем новый путь
                processing_node->children.push_back(node);
            }

            // добавление путя на конечную ноду
            if (processing_node->level + 1 == work_cost_transposed.size()) {
                processing_node->children.push_back(tail);
                endings.push_back(*processing_node);
            }

            // добавление детей обрабатываемой ноды в стэк на обработку
            for(auto & i : processing_node->children){
                stack_.push(&i);
            }
        }
    }

    static bool output_cmp(const Graph_node& A, const Graph_node& B){
        return A.cost_to_this_node < B.cost_to_this_node;
    }

    int nodes_amount(){
        return node_counter;
    }

};


#endif //DSAA_COURSEWORK_SOLUTION_H
