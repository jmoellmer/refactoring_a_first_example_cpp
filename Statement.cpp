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

    json performances = invoice["performances"];
    for (auto &perf : performances) {
        const string playID = perf["playID"];
        const json play = plays[playID];

        auto amountFor = [](const nlohmann::json& perf, const nlohmann::json& play) {
            int thisAmount = 0;
            if (play["type"] == "tragedy") {
                thisAmount = 40000;
                if (perf["audience"].get<int>() > 30) {
                    thisAmount += 1000 * (perf["audience"].get<int>() - 30);
                }
            } else if (play["type"] == "comedy") {
                thisAmount = 30000;
                if (perf["audience"].get<int>() > 20) {
                    thisAmount += 1000 + 500 * (perf["audience"].get<int>() - 20);
                }
                thisAmount += 300 * perf["audience"].get<int>();
            } else {
                throw logic_error("unknown type: " + play["type"].get<string>());
            }
            return thisAmount;
        };

        int thisAmount = amountFor(perf, play);

        // add volume credits
        volumeCredits += max(perf["audience"].get<int>() - 30, 0);
        // add extra credit for every ten comedy attendees
        if ("comedy" == play["type"]) {
            volumeCredits += floor(perf["audience"].get<int>() / 5);
        }

        // print line for this order
        resultStream << "\t" << play["name"].get<string>() << ": " << std::showbase << put_money(thisAmount / 100)
                     << " (" << to_string(perf["audience"].get<int>()) << " seats)\n";

        result += resultStream.str();
        resultStream.str("");
        resultStream.clear();
        totalAmount += thisAmount;
    }

    resultStream << "Amount owed is " << std::showbase << put_money(totalAmount / 100) << "\n";
    result += resultStream.str();
    result += "You earned " + to_string(volumeCredits) + " credits\n";
    return result;
}
