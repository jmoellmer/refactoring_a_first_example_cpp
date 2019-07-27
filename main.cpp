#include <nlohmann/json.hpp>
#include <iostream>
#include "Statement.h"

using namespace nlohmann;

int main() {

    auto plays = R"(
        {
            "hamlet": {"name": "Hamlet", "type": "tragedy"},
            "as-like": {"name": "As You Like It", "type": "comedy"},
            "othello": {"name": "Othello", "type": "tragedy"}
        }
    )"_json;

    auto invoice = R"(
            {
                "customer": "BigCo",
                "performances": [
                    {
                        "playID": "hamlet",
                        "audience": 55
                    },
                    {
                        "playID": "as-like",
                        "audience": 35
                    },
                    {
                        "playID": "othello",
                        "audience": 40
                    }
                ]
            }
    )"_json;

    std::cout << Statement::printStatement(invoice, plays) << std::endl;

    return 0;
}