#pragma once

#include "GraphParser.hpp"

class AdjacencyMatrixParser : public GraphParser {
public:
    Graph parse(const std::string& data) override;
};