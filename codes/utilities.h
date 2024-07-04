#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "circuit.h"
using namespace std;

//variable
extern unordered_map<string, int> all_literals;
extern circuit_info c1;
extern circuit_info c2;

//functions

//write all literals and their corresponding encoded number
void print_all_literals();

void get_po_support_size();

void print_time(const string& message);

double get_time();

extern chrono::steady_clock::time_point program_start_time;
#endif