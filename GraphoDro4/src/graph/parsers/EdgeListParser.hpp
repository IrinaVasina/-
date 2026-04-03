#pragma once

#include "GraphParser.hpp"

class EdgeListParser : public GraphParser {
public:
    Graph parse(const std::string& data) override;
};