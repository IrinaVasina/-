#pragma once

#include "../Graph.hpp"
#include <string>
#include <fstream>
#include <sstream>

class GraphParser {
public:
    virtual ~GraphParser() = default;

    virtual Graph parse(const std::string& data) = 0;

    Graph parseFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return parse(buffer.str());
    }
};