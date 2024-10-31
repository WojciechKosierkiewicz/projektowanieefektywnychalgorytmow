#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <random>
#include <unordered_set>
#include <__random/random_device.h>
#include "Graph.h"


class Configuration {
public:
    Configuration(std::string filename);

    bool Asymetric;
    std::string DataLocation;
    std::string OutPutFilePath;
    double MaxProcessingTime;
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


void shuffle(std::vector<int> &v) {
    auto rng = std::default_random_engine(std::random_device{}());
    std::shuffle(v.begin(), v.end(), rng);
}

TestResult Algorytm(Graph graph, std::string filename, Configuration config) {
    std::unordered_set<int> visited;
    std::vector<int> path;
    std::vector<int> tested_path;

    tested_path.reserve(graph.data.size());
    for (int i = 0; i < graph.data.size(); i++) {
        tested_path.push_back(i);
    }

    int pathcost;
    int sum_of_cost = 0;

    for (auto node: graph.data)
        for (auto &i: node.second)
            sum_of_cost += i.weight;

    int estimated_cost = sum_of_cost / graph.data.size();

    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        pathcost = 0;
        shuffle(tested_path);
        pathcost += graph.data[tested_path[tested_path.size() - 1]][tested_path[0]].weight;
        for (int i = 1; i < tested_path.size(); i++) {
            pathcost += graph.data[i - 1][tested_path[i]].weight;
        }
        if (pathcost < estimated_cost) {
            path.clear();
            path.insert(path.begin(), tested_path.begin(), tested_path.end());
            break;
        }
        if (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start).count()
            < config.MaxProcessingTime) {
            path.clear();
            path.insert(path.begin(), tested_path.begin(), tested_path.end());
            break;
        }
    }
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
        pathcost,
    };
}

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));
    Configuration config = Configuration("config.txt");
    Graph graph;
    std::vector<TestResult> results;
    for (const auto &entry: std::filesystem::directory_iterator(config.DataLocation)) {
        graph = Graph(config.DataLocation + "/" + entry.path().filename().string());
        results.push_back(Algorytm(graph, entry.path().stem(), config));
    }
    print_test_results_to_csv(results, config.OutPutFilePath);
}
