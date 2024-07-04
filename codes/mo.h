#ifndef MO_H
#define MO_H

#include <string>
#include <iostream>
#include "circuit.h"

using namespace std;

class mo{
public:
    //variables
    string mo_name;
    string c1_po;
    int c1_po_ss;
    string c2_po;
    int c2_po_ss;
    char cd;
    int i;
    int j;
    int literal_no;

    //functions
    void configure(string c1_po, string c2_po, char cd, int i, int j, string mo_name);
    
    mo(string mo_name, string c1_po, string c2_po, char cd,  int i, int j) : mo_name(mo_name), c1_po(c1_po), c2_po(c2_po), cd(cd), i(i), j(j) {}

    friend std::ostream& operator<<(std::ostream& os, const mo& m);
};

extern circuit_info c1;
extern circuit_info c2;

#endif