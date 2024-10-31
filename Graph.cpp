//
// Created by Wojtek Kosierkiewicz on 31/10/2024.
//

#include "Graph.h"

#include <fstream>
#include <sstream>

void Graph::print() {
    for (auto node: data) {
        for (int i = 0; i < node.second.size(); i++) {
            std::cout << node.first << " " << node.second[i].weight << std::endl;
        }
    }
}

void Graph::import_from_file(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "nie znaleziono pliku :( " + filename << "\n";
    }
    std::string line;
    int row = 0;
    int col = 0;
    int num;
    while (std::getline(file, line)) {
        col = 0;
        std::istringstream ststream(line);
        while (ststream >> num) {
            data[row].push_back(Edge{num, col});
            col++;
        }
        row++;
    }
    for (auto &edge: data) {
        std::sort(edge.second.begin(), edge.second.end());
    }
}
