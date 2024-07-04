#ifndef OUTPUT_SOLVER_H
#define OUTPUT_SOLVER_H

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include "mi_generator.h"
#include "mi.h"
#include "utilities.h"
#include "constraints_generator.h"
#include "cadical.hpp"

using namespace std;

//variables
extern mi_info input_mapping_info;
extern mo_info output_mapping_info;

//functions
void output_solver();
string output_sat();
void sat_test_output();
void sat_test_input();
#endif