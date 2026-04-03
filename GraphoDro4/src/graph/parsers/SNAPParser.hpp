#pragma once

#include "GraphParser.hpp"

class SNAPParser : public GraphParser {
public:
    Graph parse(const std::string& data) override;
};