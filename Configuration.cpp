#include "Configuration.h"

bool Configuration::getBool(std::string key) {
    if (config[key] == "true") {
        return true;
    } else if (config[key] == "false") {
        return false;
    }
    throw std::invalid_argument("Configuration::getBool: Key " + key + " does not exist");
}

int Configuration::getInt(std::string key) {
    try {
        return std::stoi(config[key]);
    } catch (std::invalid_argument &e) {
        throw std::invalid_argument("Configuration::getInt: Key " + key + "is not valid");
    }
}

double Configuration::getDouble(std::string key) {
    try {
        return std::stod(config[key]);
    } catch (std::invalid_argument &e) {
        throw std::invalid_argument("Configuration::getDouble: Key " + key + " is not valid");
    }
}

float Configuration::getFloat(std::string key) {
    try {
        return std::stof(config[key]);
    } catch (std::invalid_argument &e) {
        throw std::invalid_argument("Configuration::getDouble: Key " + key + " is not valid");
    }
}

std::string Configuration::getString(std::string key) {
    return config[key];
}


