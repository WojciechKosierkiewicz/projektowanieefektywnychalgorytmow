//
// Created by Wojtek Kosierkiewicz on 31/10/2024.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <list>
#include <map>
#include <iostream>

struct Edge {
    int weight;
    int y;

    bool operator<(const Edge &other) const {
        return weight < other.weight;
    }
};

class Graph {
public:
    std::map<int, std::vector<Edge> > data;

    Graph() {
    }

    Graph(std::string filename) {
        import_from_file(filename);
    }

    void import_from_file(std::string filename);

    void print();
};


#endif //GRAPH_H
