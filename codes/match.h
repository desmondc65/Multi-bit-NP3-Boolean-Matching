#ifndef MATCH_H
#define MATCH_H

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

using namespace std;

class IN_Group;
class OUT_Group;
class CONST_Group;

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
    void setMatch(vector<pair<pair<string, bool>, pair<string, bool>>>& pi1);

    // print matching info
    void printMatch() const;
};

#endif