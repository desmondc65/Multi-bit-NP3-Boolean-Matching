
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include <sys/stat.h>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <utility>
#include <set>

#ifndef match_h
#define match_h
#endif

using namespace std;

//elements for mappings
class Match {
private:
    string circuit_1_po_name;
    string circuit_2_po_name;
    bool circuit_1_negation;
    bool circuit_2_negation;
    vector<pair<pair<string, bool>, pair<string, bool>>> pi_match;

public:
    Match(const string& poName1, const string& poName2, bool negation1, bool negation2, vector<pair<pair<string, bool>, pair<string, bool>>>& pi1)
        : circuit_1_po_name(poName1),
          circuit_2_po_name(poName2),
          circuit_1_negation(negation1),
          circuit_2_negation(negation2),
          pi_match(pi1)
          {
    }

    // Member function to set the private variables
    void setMatch(vector<pair<pair<string, bool>, pair<string, bool>>>& pi1) {
        pi_match = pi1;
    }

    void printMatch() const {
        cout << "Circuit 1 PO Name: " << circuit_1_po_name << endl;
        cout << "Circuit 2 PO Name: " << circuit_2_po_name << endl;
        cout << "Circuit 1 Negation: " << (circuit_1_negation ? "True" : "False") << endl;
        cout << "Circuit 2 Negation: " << (circuit_2_negation ? "True" : "False") << endl;

        cout << "PI Match: " << endl;
        for (const auto& pi : pi_match) {
            cout << "PI 1 - Name: " << pi.first.first << ", Negation: " << (pi.first.second ? "True" : "False") << " and ";
            cout << "PI 2 - Name: " << pi.second.first << ", Negation: " << (pi.second.second ? "True" : "False") << endl;
        }
    }
};
vector<Match> matches;
int match_count = 0;
