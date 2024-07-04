#ifndef MO_GENERATOR_H
#define MO_GENERATOR_H

#include <iostream>
#include <vector>
#include "mo.h"

using namespace std;

//construct mo mapping
vector<vector<mo>> build_mo();

//print mi mapping
void mo_printer(vector<vector<mo>>& mo_mapping);

#endif