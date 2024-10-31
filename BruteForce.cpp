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

void reverse_vec(std::vector<int> &arr, int begin, int end) {
    int temp;
    while (begin < end) {
        temp = arr[begin];
        arr[begin] = arr[end];
        arr[end] = temp;
        begin++;
        end--;
    }
}


bool permutuj(std::vector<int> &arr) {
    bool pokolei = false;
    for (int i = 1; i < arr.size(); ++i)
        if (arr[i - 1] - arr[i] < 0)
            pokolei = true;

    if (!pokolei) {
        return false;
    }
    int pivot = -1;
    for (int i = arr.size() - 2; i >= 0; --i) {
        if (arr[i] < arr[i + 1]) {
            pivot = i;
            break;
        }
    }
    if (pivot == -1) {
        reverse_vec(arr, 0, arr.size() - 1);
        return true;
    }
    int temp;
    for (int i = arr.size() - 1; i > pivot; i--) {
        if (arr[pivot] < arr[i]) {
            temp = arr[i];
            arr[i] = arr[pivot];
            arr[pivot] = temp;
            break;
        }
    }
    reverse_vec(arr, pivot + 1, arr.size() - 1);

    return true;
}

TestResult Algorytm(Graph graph, std::string filename) {
    std::unordered_set<int> visited;
    std::vector<int> path;
    std::vector<int> tested_path;

    // for (int i = 4; i > 0; i--) {
    //     tested_path.push_back(i);
    // }
    tested_path.reserve(graph.data.size());
    for (int i = 0; i < graph.data.size(); i++) {
        tested_path.push_back(i);
    }
    // tested_path.push_back(10);

    int pathcost = 0;
    int mincost = INT_MAX;
    auto start = std::chrono::high_resolution_clock::now();
    do {
        pathcost = 0;
        pathcost += graph.data[tested_path[tested_path.size() - 1]][tested_path[0]].weight;
        for (int i = 1; i < tested_path.size(); i++) {
            pathcost += graph.data[i - 1][tested_path[i]].weight;
        }
        if (pathcost < mincost) {
            path.clear();
            path.insert(path.begin(), tested_path.begin(), tested_path.end());
            mincost = pathcost;
        }
    } while (permutuj(tested_path));
    auto stop = std::chrono::high_resolution_clock::now();
    auto len = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "problem " << filename.c_str() << std::endl;
    std::cout << "koszt " << mincost << std::endl;
    for (int i = 0; i < tested_path.size(); i++) {
        std::cout << tested_path[i] << " ";
    }
    std::cout << std::endl;

    return
            TestResult{
                std::to_string(len.count()),
                filename,
                path,
                mincost,
            };
}

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));
    Configuration config = Configuration("config.txt");
    Graph graph;
    std::vector<TestResult> results;
    for (const auto &entry: std::filesystem::directory_iterator(config.DataLocation)) {
        graph = Graph(config.DataLocation + "/" + entry.path().filename().string());
        results.push_back(Algorytm(graph, entry.path().stem()));
    }
    print_test_results_to_csv(results, config.OutPutFilePath);
}
