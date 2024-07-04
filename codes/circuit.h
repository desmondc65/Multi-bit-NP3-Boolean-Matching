#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iterator>
#include <algorithm>
#include "po.h"

using namespace std;

class circuit_info{
public:
    //variables
    vector<string> pis_name;
    vector<string> pos_name;
    vector<string> assigns;
    vector<string> wire;
    vector<pair<string, string>> bus_input;
    vector<int> bus_size_input;
    vector<po> pos;

    string path_v;
    string new_path;
    string folder_name;
    string simplified_v;
    string aig_path;
    string cnf_path;
    string name;

    int no_of_pis, no_of_pos;

    //cnf variables
    vector<vector<int>> clauses;
    unordered_map<string, int> literals;

    unordered_map<string, int> po_support_size;

    //functions
    circuit_info(string no); //constructor

    void cir_print();

    void verilog_to_cnf();

    void print_assign();

    void print_wire();

    void dissect();

    void get_wire(string& line);

    void print_clauses();

    void write_literals_to_file();

    void sort_circuit_po();

    void print_each_po_cnf();

};

extern long long int literal_no;
extern unordered_map<string, int> all_literals;


#endif