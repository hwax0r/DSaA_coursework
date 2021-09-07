//
// Created by David Sergeev on 07.09.2021.
//

#ifndef DSAA_COURSEWORK_GRAPHNODE_H
#define DSAA_COURSEWORK_GRAPHNODE_H

#include "iostream"
#include "map"
#include "vector"

struct Graph_node{
    std::string name; // название ноды
    int worker_cost; // вес
    int level; // уровень("задача")

    // possibilities - содержит все возможные следующие пути и их стоимость
    std::map<std::string, int>  possibilities;

    // children - набор всех наследников ноды
    std::vector<Graph_node> children;

    // путь до каждого элкмента
    std::vector<std::string> route;
    int cost_to_this_node = 0;


    std::vector<std::string> keys_to_be_deleted;



    Graph_node(){}
    Graph_node(std::string name, int cost_to_this,
               int level, std::map<std::string, int> possibilities){
        this->name = name;
        this->worker_cost = cost_to_this;
        this->level = level;
        this->possibilities = possibilities;
    }
    Graph_node(std::string name, int cost_to_this, int level){
        this->name = name;
        this->worker_cost = cost_to_this;
        this->level = level;
    }
};


#endif //DSAA_COURSEWORK_GRAPHNODE_H
