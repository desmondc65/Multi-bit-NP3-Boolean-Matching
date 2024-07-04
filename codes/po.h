#ifndef PO_H
#define PO_H

#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class po{
public:
    string po_name;
    int support_size;
    int literal_no;
    unordered_set<string> var;
    vector<vector<int>> clauses;

    //constructor
    po(string po_name, int support_size) : po_name(po_name), support_size(support_size) {}
};
#endif