//
// Created by Jeff Moellmer on 2019-07-27.
//

#include "Statement.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace nlohmann;

std::string Statement::printStatement(const nlohmann::json &invoice, const nlohmann::json &plays) {
    int totalAmount = 0;
    string customer;
    string result = "Statement for " + invoice["customer"].get<string>() + "\n";

    // function playFor()
    auto playFor = [plays](const nlohmann::json& aPerformance) {
        return plays[aPerformance["playID"].get<string>()];
    };

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

    // function volumeCredits()
    auto volumeCreditsFor = [playFor](const nlohmann::json& aPerformance) {
        int result = 0;
        result += max(aPerformance["audience"].get<int>() - 30, 0);
        if ("comedy" == playFor(aPerformance)["type"]) {
            result += floor(aPerformance["audience"].get<int>() / 5);
        }
        return result;
    };

    // function totalVolumeCredits()
    auto totalVolumeCredits = [invoice, volumeCreditsFor]() {
        int volumeCredits = 0;
        for (auto &perf : invoice["performances"]) {
            volumeCredits += volumeCreditsFor(perf);
        }
        return volumeCredits;
    };

    for (auto &perf : invoice["performances"]) {

        // print line for this order
        result += "\t" + playFor(perf)["type"].get<string>() + ": " + usd(amountFor(perf))
                  + " (" + to_string(perf["audience"].get<int>()) + " seats)\n";
        totalAmount += amountFor(perf);
    }

    result += "Amount owed is " + usd(totalAmount) + "\n";
    result += "You earned " + to_string(totalVolumeCredits()) + " credits\n";
    return result;
}

std::string Statement::usd(float aNumber) {
    ostringstream ss;
    ss.imbue(locale("en_US.UTF-8"));
    ss << std::showbase << put_money(aNumber / 100);
    return ss.str();
}
