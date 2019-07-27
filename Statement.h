//
// Created by Jeff Moellmer on 2019-07-27.
//

#pragma once
#include <nlohmann/json.hpp>

class Statement {
public:
    Statement() = default;
    static std::string printStatement(const nlohmann::json &invoice, const nlohmann::json &plays);
};



