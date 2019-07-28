//
// Created by Jeff Moellmer on 2019-07-27.
//

#include "Statement.h"
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace nlohmann;

std::string Statement::printStatement(const nlohmann::json &invoice, const nlohmann::json &plays) {
    int totalAmount = 0;
    int volumeCredits = 0;
    string customer;
    string result = "Statement for " + invoice["customer"].get<string>() + "\n";
    ostringstream resultStream;
    resultStream.imbue(locale("en_US.UTF-8"));

    // function playFor()
    auto playFor = [plays](const nlohmann::json& aPerformance) {
        const string playID = aPerformance["playID"];
        return plays[playID];
    };

    json performances = invoice["performances"];
    for (auto &perf : performances) {

        // function amountFor()
        auto amountFor = [playFor](const nlohmann::json& aPerformance) {
            int result = 0;
            if (playFor(aPerformance)["type"] == "tragedy") {
                result = 40000;
                if (aPerformance["audience"].get<int>() > 30) {
                    result += 1000 * (aPerformance["audience"].get<int>() - 30);
                }
            } else if (playFor(aPerformance)["type"] == "comedy") {
                result = 30000;
                if (aPerformance["audience"].get<int>() > 20) {
                    result += 1000 + 500 * (aPerformance["audience"].get<int>() - 20);
                }
                result += 300 * aPerformance["audience"].get<int>();
            } else {
                throw logic_error("unknown type: " + playFor(aPerformance)["type"].get<string>());
            }
            return result;
        };

        // function volumeCredits
        auto volumeCreditsFor = [playFor](const nlohmann::json& aPerformance) {
            int result = 0;
            result += max(aPerformance["audience"].get<int>() - 30, 0);
            if ("comedy" == playFor(aPerformance)["type"]) {
                result += floor(aPerformance["audience"].get<int>() / 5);
            }
            return result;
        };

        volumeCredits += volumeCreditsFor(perf);

        // print line for this order
        resultStream << "\t" << playFor(perf)["type"].get<string>() << ": " << std::showbase << put_money(amountFor(perf) / 100)
                     << " (" << to_string(perf["audience"].get<int>()) << " seats)\n";

        result += resultStream.str();
        resultStream.str("");
        resultStream.clear();
        totalAmount += amountFor(perf);
    }

    resultStream << "Amount owed is " << std::showbase << put_money(totalAmount / 100) << "\n";
    result += resultStream.str();
    result += "You earned " + to_string(volumeCredits) + " credits\n";
    return result;
}
