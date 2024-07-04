#ifndef BMATCH_H
#define BMATCH_H

#define debug 0

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include <sys/stat.h>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <utility>
#include "match.h"
#include "preprocess.h"
#include "verilog_parser.h"
#include "mi.h"
#include "mi_generator.h"
#include "mo.h"
#include "mo_generator.h"
#include "circuit.h"
#include "constraints_generator.h"
#include "utilities.h"
#include "projection.h"
#include "output_solver.h"

using namespace std;

#if defined(ABC_NAMESPACE)
namespace ABC_NAMESPACE
{
#elif defined(__cplusplus)
extern "C"
{
#endif

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
void   Abc_Start();
void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
typedef struct Abc_Frame_t_ Abc_Frame_t;
Abc_Frame_t * Abc_FrameGetGlobalFrame();
int Cmd_CommandExecute( Abc_Frame_t * pAbc, const char * sCommand );
#if defined(ABC_NAMESPACE)
}
using namespace ABC_NAMESPACE;
#elif defined(__cplusplus)
}
#endif

//****************variables****************//

//basic variables
circuit_info c1 = circuit_info("c1");
circuit_info c2 = circuit_info("c2");

//check match existed
vector<vector<bool>> po_match_table;

//constraint generator variable
mi_info input_mapping_info;
mo_info output_mapping_info;

//literal no
long long int literal_no;

//collection of literal no pair
unordered_map<string, int> all_literals;

//timer
chrono::steady_clock::time_point program_start_time = chrono::steady_clock::now();

//****************functions****************//

//preprocess functions

#endif