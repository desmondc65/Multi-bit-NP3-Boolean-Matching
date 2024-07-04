#ifndef PROJECTION_H
#define PROJECTION_H

#include <iostream>
#include <vector>
#include "fstream"
#include <sys/types.h>
#include <sys/stat.h>
#include "circuit.h"
#include "mo_generator.h"
#include "constraints_generator.h"
#include "mo.h"

using namespace std;

//variables
extern long long int literal_no;
extern circuit_info c1;
extern circuit_info c2;
extern mo_info output_mapping_info;
extern unordered_map<string, int> all_literals;

//functions
void configure_projection();
void print_projection_clauses();
#endif