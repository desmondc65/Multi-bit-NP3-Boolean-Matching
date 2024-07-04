#ifndef VERILOG_PARSER_H
#define VERILOG_PARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <utility>
#include <sys/types.h>
#include <sys/stat.h>
#include "circuit.h"

using namespace std;

//get pi name
void get_pi(string line, vector<string>& vec_pis);

//parse verilog file
void verilog_parser(circuit_info& cir);

//separate pis and commands of each po
void write_pi_po_assigns(circuit_info& cir);
#endif