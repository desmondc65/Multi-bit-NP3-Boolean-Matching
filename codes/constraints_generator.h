#ifndef CONSTRAINTS_GENERATOR_H
#define CONSTRAINTS_GENERATOR_H

#include "mi.h"
#include "mo.h"
#include "vector"
#include "unordered_map"
#include "circuit.h"
#include "fstream"
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

class mi_info{
public:
    //variables
    unordered_map<string, int> mi_mapping; //store mi mapping variables and their corresponding literal no
    vector<vector<int>> mi_clauses_3; //constraint 3 clauses
    vector<vector<int>> mi_clauses_5; //constraint 5 clauses

    //input mapping matrix, mi
    vector<vector<mi>> input_mapping;

    //functions
    //constraint (3) of NP3 Boolean Matching: Ensures every input of g should be mapped to either an input of f or constant
    void mi_constraints_generator(const vector<vector<mi>>& input_mapping);
 
    //constraint (5): MI matrix, bound f and g as MI, assign literal no
    void mi_mapper(vector<vector<mi>>& input_mapping); 
    
    unordered_map<string, int> mi_literals;

    //write mi clauses to a file
    void print_mi_clauses();

    void mi_printer(const vector<vector<mi>>& input_mapping);
};

class mo_info{
    public:
    //variables;
    unordered_map<string, int> mo_mapping;
    vector<vector<int>> mo_clauses_1; //constraint 1 clauses
    vector<vector<int>> mo_clauses_2; //constraint 2 clauses
    vector<vector<int>> mo_clauses_4; //constraint 4 clauses

    bool projection_on = false;
    
    //output mapping matrix, mo
    vector<vector<mo>> output_mapping;

    //allowProj clauses
    vector<vector<int>> projection_clauses;
    vector<int> allow_proj_off;
    vector<int> allow_proj_on;

    //dont pick entire row clauses;
    vector<vector<int>> picked_mo;

    unordered_map<string, int> mo_literals;

    //functions
    //constraint (4): MO matrix, assign literal no to c's and d's
    void mo_mapper(vector<vector<mo>>& output_mapping);

    //constraint (1): at least a pair of outputs should be matched
    //constraint (2): an output of g can be mapped to at most one output of f 
    void mo_constraints_generator(const vector<vector<mo>>& output_mapping);
    
    void print_mo_clauses();

    void mo_printer(const vector<vector<mo>>& output_mapping);
};

extern long long int literal_no;
extern unordered_map<string, int> all_literals;

#endif