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
#include "po_pi.h"
#include "match.h"
#include "preprocess.h"
#include "verilog_parser.h"
#include "utilities.h"

#ifndef bmathc_h
#define bmathc_h
#endif

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
int c1_number_of_inputs, c2_number_of_inputs;
string c1_path_v, c2_path_v;
string c1_new_path_v = "c1";
string c2_new_path_v = "c2";
vector<int> c1_bus_size_input, c2_bus_size_input; 
vector<string> c1_bus_input_1, c1_bus_input_2;
vector<string> c2_bus_input_1, c2_bus_input_2;
string c1_folder = "c1_data";
string c2_folder = "c2_data";
string c1_name = "c1_";
string c2_name = "c2_";

//verilog assign lines
vector<string> c1_verilog_assigns;
vector<string> c2_verilog_assigns;
vector<string> c1_vec_pis, c2_vec_pis;
vector<string> c1_vec_pos, c2_vec_pos;

//check match existed
vector<vector<bool>> po_match_table;

//cir 1 and 2's pos
vector<c1_PO> c1_Pos;
vector<c2_PO> c2_Pos;

//****************functions****************//

//preprocess functions
void read_input(string& input_name, string& c1_path_v, int& c1_number_of_inputs, string& c2_path_v, int& c2_number_of_inputs, vector<int>& c1_bus_size_input, vector<int>& c2_bus_size_input, vector<string>& c1_bus_input_1, vector<string>& c2_bus_input_1, vector<string>& c1_bus_input_2, vector<string>& c2_bus_input_2);
void prepare_input(string&intput_circuit_path, string& output_circuit_path);
void read_strash_aig_cnf(Abc_Frame_t * pAbc);

//verilog parser
void verilog_parser(string intput_circuit_path, vector<string>& verilog_assigns, vector<string>& vec_pis, vector<string>& vec_pos, string cir_name);
void get_pi(string line, vector<string>& vec_pis, unordered_set<string>& poset);

//separate pis and commands of each po
void creater_pointers_to_po();
void write_pi_po_assigns(vector<string>& pi, vector<string>& po, string folder, string cir_name);
void sort_assigns_to_po1(vector<c1_PO> &circuit_po_self, vector<string>& vec_po, vector<string>& circuit_verilog_assigns, string& folder, string& circ);
void sort_assigns_to_po2(vector<c2_PO> &circuit_po_self, vector<string>& vec_po, vector<string>& circuit_verilog_assigns, string& folder, string& circ);
void extract_wires_pi1(string line, vector<c1_PO> &circuit__po_self);
void extract_wires_pi2(string line, vector<c2_PO> &circuit__po_self);

//two-step NP3 boolean matcher
void output_solver(vector<Primary_Output>& c1, vector<Primary_Output>& c2, Abc_Frame_t * pAbc);

//utilities
bool cecTest(string cir1, string cir2, Abc_Frame_t * pAbc);
bool readFileEquivalent(string path);

bool input_solver(c1_PO& c1_po, c2_PO& c2_po, Abc_Frame_t * pAbc, int i, int j){
    string folderName1 =   "c1_each_po/";
    string cir1_path = folderName1 + c1_po.name() + ".v";

    string folderName2 =   "c2_each_po/";
    string cir2_path = folderName2 + c2_po.name() + ".v";

    vector<string> c1_inputs = c1_po.get_pi();
    vector<string> c2_inputs = c2_po.get_pi();
    
    c1_po.set_pi_test_config();

    vector<vector<int>> c1_input_negate = enumerateBits(c1_po.pi_size());
    int c1_input_negate_no = c1_input_negate.size();

    vector<vector<int>> c2_input_permutation = generatePermutations(c2_po.pi_size());
    
    int c2_input_permute_no = c2_input_permutation.size();

    vector<vector<int>> c2_input_negate;
    int c2_input_negate_no = c2_input_negate.size();
    if(c1_po.pi_size() == c2_po.pi_size()){
        c2_input_negate = c1_input_negate;
        c2_input_negate_no = c1_input_negate_no;
    }
    else{
        c2_input_negate = enumerateBits(c2_po.pi_size());
        c2_input_negate_no = c2_input_negate.size();
    }
    int iteration = 0;
    for(int i = 0; i < c1_input_negate_no; i++){//negation numeration of c1_input
        for(int j = 0; j < c2_input_permute_no; j++){
            for(int k = 0; k < c2_input_negate_no; k++){  
                iteration++;
                //set c1_pis negate pattern
                c1_po.set_pi_negate_config(c1_input_negate[i]);

                //set c2_pis permutation and negation pattern
                c2_po.set_c2_pi_permute_config(c2_input_permutation[j]);
                c2_po.set_pi_negate_config(c2_input_negate[k]);

                //set new names of c2 pis
                c2_po.set_pi_test_config();

                //write test verilog of c1 po and c2 po
                c1_po.write_test_verilog(cir1_path);
                c2_po.write_test_verilog(cir2_path);
                
                if(cecTest(cir1_path, cir2_path, pAbc)){
                    cout << "found during " << iteration << endl;
                    cout << "c1_input_negate[i]" << endl;
                    for(auto i: c1_input_negate[i]){
                        cout << i << " " ;
                    }
                    cout << endl;
                    cout << "c2_input_permutation[j]" << endl;
                    for(auto i: c2_input_permutation[j]){
                        cout << i << " " ;
                    }
                    cout << endl;
                    cout << "c2_input_negation[k]" << endl;
                    for(auto i: c2_input_negate[k]){
                        cout << i << " " ;
                    }
                    cout << endl;
                    po_match_table[i][j] = true;
                    po_match_table[j][i] = true;
                    cout << c1_po.name() << " and " << c2_po.name() << " are matched" << endl; 
                    pair<string, bool> c1_pi_info;
                    pair<string, bool> c2_pi_info;
                    vector<pair<pair<string, bool>, pair<string, bool>>> pimatch;
                    for(int g = 0; g < c1_po.pi_size(); g++){
                        cout << "g is " << g << endl;
                        c1_pi_info = make_pair(c1_po.pi_name(g), c1_po.pi_negation(g));
                        cout << c1_po.pi_name(g) << " c1 " << c1_po.pi_negation(g) << endl;
                        c2_pi_info = make_pair(c2_po.pi_name(c2_po.pi_no(g)), c2_po.pi_negation(c2_po.pi_no(g)));
                        cout << c2_po.pi_name(c2_po.pi_no(g)) << " c2 " << c2_po.pi_negation(c2_po.pi_no(g)) << endl;
                        pair<pair<string, bool>, pair<string, bool>> temp = make_pair(c1_pi_info, c2_pi_info);
                        pimatch.emplace_back(temp);
                    }
                    matches.emplace_back(c1_po.name(), c2_po.name(), c1_po.po_negation(), c2_po.po_negation(), pimatch);
                    matches[match_count].printMatch();
                    match_count++;
                    return true;
                }
            }
        }
    }
    return false;
}

void output_solver(vector<c1_PO>& c1, vector<c2_PO>& c2, Abc_Frame_t * pAbc){
    cout << "*********************************************" << endl << endl;
    cout << "bmatching..." << endl;

    int c1_sz = c1.size();
    cout << "c1 has " << c1_sz << " pos" << endl;

    int c2_sz = c2.size();
    cout << "c2 has " << c2_sz << " pos" << endl;

    // resize po match table
    po_match_table.resize(c1.size());
    for(int i = 0; i < c1.size(); i++){
        po_match_table[i].resize(c2.size(), false);
    }
    //enumerate through po
    for(int i = 0; i < c1_sz; i++){
        for(int j = 0; j < c2_sz; j++){
            //these pos were matched
            if(po_match_table[i][j] == true || po_match_table[j][i] == true){
                continue;
            }
            int c1_pi_size = c1[i].pi_size();
            int c2_pi_size = c2[j].pi_size();
            if(c1_pi_size != c2_pi_size){
                continue;
            }
            for(int k = 0; k <= 1; k++){
                if(po_match_table[i][j] == true || po_match_table[j][i] == true){
                    break;
                }
                if(k == 0){
                    c1[i].negate(false);
                }
                else{
                    c1[i].negate(true);
                }
                for(int l = 0; l <= 1; l++){
                    if(po_match_table[i][j] == true || po_match_table[j][i] == true){
                        break;
                    }
                    if(k == 0){
                        c1[i].negate(false);
                    }
                    else{
                        c1[i].negate(true);
                    }
                    if(input_solver(c1[i], c2[j], pAbc, i, j)){
                        po_match_table[i][j] = true;
                        po_match_table[j][i] = true;
                    }
                    if(po_match_table[i][j] == true || po_match_table[j][i] == true){
                        break;
                    }
                }
                if(po_match_table[i][j] == true || po_match_table[j][i] == true){
                    break;
                }
            }
            po_match_table[i][j] = true;
            po_match_table[j][i] = true;
        }
    }
}
