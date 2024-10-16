#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <filesystem>

class Configuration {
public:
    Configuration(std::string filename);

    bool Asymetric;
    std::string DataLocation;
    int size;
    int amount;
    int maxweight;
    int minweight;

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
        if (key == "DataOutputLocation") {
            DataLocation = option;
        } else if (key == "Asymetric") {
            if (option == "true") {
                Asymetric = true;
            }
            if (option == "false") {
                Asymetric = false;
            }
        } else if (key == "size") {
            size = std::stoi(option);
        } else if (key == "amount") {
            amount = std::stoi(option);
        } else if (key == "maxweight") {
            maxweight = std::stoi(option);
        } else if (key == "minweight") {
            minweight = std::stoi(option);
        } else {
            std::cout << "Invalid config line: " << line << std::endl;
        }
    }
}

void create_problem(Configuration config, std::string filename) {
    std::ofstream file(filename);
    for (int i = 0; i < config.size; i++) {
        for (int j = 0; j < config.size; j++) {
            file << config.minweight + (rand() % (config.maxweight - config.minweight)) << " ";
        }
        file << std::endl;
    }
}

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));
    Configuration config = Configuration("problemconfig.txt");
    int largest = 0;
    for (const auto &entry: std::filesystem::directory_iterator(config.DataLocation)) {
        largest = std::max(largest, stoi(entry.path().stem().string()));
    }
    for (int i = 0; i < config.amount; i++) {
        create_problem(config, config.DataLocation + "/" + std::to_string(largest + i) + ".txt");
    }
}
