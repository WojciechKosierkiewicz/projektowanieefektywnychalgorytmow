//
// Created by Wojtek Kosierkiewicz on 31/10/2024.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <string>
#include <map>

#endif //CONFIGURATION_H
class Configuration {
    Configuration(std::string filename);

    std::map<std::string, std::string> config;

public:
    int getInt(std::string key);

    std::string getString(std::string key);

    bool getBool(std::string key);

    double getDouble(std::string key);

    float getFloat(std::string key);
};
