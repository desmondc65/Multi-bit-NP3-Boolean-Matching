#ifndef MI_GENERATOR_H
#define MI_GENERATOR_H

#include <iostream>
#include <vector>
#include "mi.h"

using namespace std;

//construct mi mapping
vector<vector<mi>> build_mi(vector<string>& c1_pi, vector<string>& c2_pi);

//print mi mapping
void mi_printer(vector<vector<mi>>& mi_mapping);

#endif