#pragma once

#include "GraphParser.hpp"

class DIMACSParser : public GraphParser {
public:
    Graph parse(const std::string& data) override;
};