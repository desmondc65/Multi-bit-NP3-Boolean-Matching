#include "mo.h"

std::ostream& operator<<(std::ostream& os, const mo& m){
    os << m.c1_po << " " << m.c2_po << " " << m.mo_name << " " << m.i << " " << m.j;
    return os;
}

void mo::configure(string c1_po, string c2_po, char cd, int i, int j, string mo_name){
    this->c1_po = c1_po;
    this->c2_po = c2_po;
    this->cd = cd;
    this->i = i;
    this->j = j;
    this->mo_name = mo_name;
}