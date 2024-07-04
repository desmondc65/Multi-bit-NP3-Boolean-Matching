#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include "circuit.h"

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

void read_input(string& input_name, circuit_info& c1, circuit_info& c2);

string removeSecondWord(const string& str);

void prepare_input(circuit_info& cir);

void read_strash_aig_cnf(Abc_Frame_t* pAbc, circuit_info& cir);

#endif