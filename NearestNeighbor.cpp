#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include "Graph.h"


class Configuration {
public:
    Configuration(std::string filename);

    bool Asymetric;
    std::string DataLocation;
    std::string OutPutFilePath;
    double MaxProcessingTime;

    void print();
};

Configuration::Configuration(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    std::string key;
    std::string option;
    while (std::getline(file, line)) {
        key = line.substr(0, line.find(':'));
        option = line.substr(line.find(':') + 1);
        if (key == "DataLocation") {
            DataLocation = option;
        } else if (key == "MaxProcessingTime") {
            MaxProcessingTime = std::stod(option);
        } else if (key == "OutPutFilePath") {
            OutPutFilePath = option;
        } else if (key == "Asymetric") {
            if (option == "true") {
                Asymetric = true;
            }
            if (option == "false") {
                Asymetric = false;
            }
        } else {
            std::cout << "Invalid config line: " << line << std::endl;
        }
    }
}

void Configuration::print() {
    std::printf("%s\n", DataLocation.c_str());
    std::printf("Max Processing Time: %f\n", MaxProcessingTime);
    if (Asymetric) {
        std::printf("considering graph as asymetric");
    } else {
        std::printf("considering graph as symetric");
    }
}

struct TestResult {
    std::string time;
    std::string filename;
    std::vector<int> path;
    int pathcost = 0;
};

void print_test_results_to_csv(std::vector<TestResult> testresults, std::string filepath) {
    std::ofstream outputfile(filepath);

    if (!outputfile.is_open()) {
        std::cout << "Couldnt create output fil";
    }
    outputfile << "Czas;Nazwa Problemu;Koszt Sciezk;Sciezka\n";
    for (auto result: testresults) {
        outputfile << result.time << ";"
                << result.filename << ";"
                << result.pathcost << ";";
        for (int i = 0; i < result.path.size(); i++) {
            outputfile << result.path[i] << ";";
        }
        outputfile << "\n";
    }
}

TestResult NearestNeighbor(Graph graph, std::string filename) {
    std::unordered_set<int> visited;
    std::vector<int> path;
    int pathcost = 0;
    auto start = std::chrono::high_resolution_clock::now();
    int current_node = rand() % graph.data.size();
    path.push_back(current_node);
    visited.insert(current_node);
    int i;
    while (visited.size() != graph.data.size()) {
        i = 0;
        while (visited.contains(graph.data[current_node][i].y)) {
            i++;
        }
        pathcost += graph.data[current_node][i].weight;
        visited.insert(graph.data[current_node][i].y);
        current_node = graph.data[current_node][i].y;
        path.push_back(current_node);
    }
    pathcost += graph.data[i][path[0]].weight;
    auto stop = std::chrono::high_resolution_clock::now();
    auto len = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "problem " << filename.c_str() << std::endl;
    std::cout << "koszt " << pathcost << std::endl;
    for (int i = 0; i < path.size(); i++) {
        std::cout << path[i] << " ";
    }
    std::cout << std::endl;
    return TestResult{
        std::to_string(len.count()),
        filename,
        path,
        pathcost
    };
}

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));
    Configuration config = Configuration("config.txt");
    Graph graph;
    graph.print();
    std::vector<TestResult> results;
    for (const auto &entry: std::filesystem::directory_iterator(config.DataLocation)) {
        graph = Graph(config.DataLocation + "/" + entry.path().filename().string());
        results.push_back(NearestNeighbor(graph, entry.path().stem()));
    }
    print_test_results_to_csv(results, config.OutPutFilePath);
}
