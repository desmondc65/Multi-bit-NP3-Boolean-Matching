#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#ifndef preprocess_h
#define preprocess_h
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

void read_input(string& input_name, string& c1_path_v, int& c1_number_of_inputs,string& c2_path_v, int& c2_number_of_inputs, vector<int>& c1_bus_size_input, vector<int>& c2_bus_size_input, vector<string>& c1_bus_input_1, vector<string>& c2_bus_input_1, vector<string>& c1_bus_input_2, vector<string>& c2_bus_input_2){

    ifstream infile;
    infile.open(input_name);
    if(!infile){
        std::cout << "Unable to open file" << endl;
        exit(1);
    }
    
    //read circ 1 info
    infile >> c1_path_v;
    infile >> c1_number_of_inputs;
    string line;
    c1_bus_size_input.resize(c1_number_of_inputs);
    c1_bus_input_1.resize(c1_number_of_inputs);
    c1_bus_input_2.resize(c1_number_of_inputs);
    getline(infile, line);
    for(int i = 0; i < c1_number_of_inputs; i++){
        getline(infile, line);
        istringstream iss(line);
        iss >> c1_bus_size_input[i] >> c1_bus_input_1[i] >> c1_bus_input_2[i];
        //cout << c1_bus_size_input[i] << c1_bus_input_1[i] << c1_bus_input_2[i];
        //cout << line << endl;
    }

    //read circ 2 info
    infile >> c2_path_v;
    infile >> c2_number_of_inputs;
    c2_bus_size_input.resize(c1_number_of_inputs);
    c2_bus_input_1.resize(c1_number_of_inputs);
    c2_bus_input_2.resize(c1_number_of_inputs);
    getline(infile, line);
    for(int i = 0; i < c2_number_of_inputs; i++){
        getline(infile, line);
        istringstream iss(line);
        iss >> c2_bus_size_input[i] >> c2_bus_input_1[i] >> c2_bus_input_2[i];
        //cout << c2_bus_size_input[i] << c2_bus_input_1[i] << c2_bus_input_2[i];
        //cout << line << endl;
    }
    infile.close();
}

string removeSecondWord(const string& str) {
    istringstream iss(str);
    string word;
    string result;
    int count = 0;
    while (iss >> word) {
        if (count != 1) {
            result += word + " ";
        }
        count++;
    }
    // Remove trailing space if any
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    return result;
}

void prepare_input(string&intput_circuit_path, string& output_circuit_path){
    ifstream inputFile(intput_circuit_path);
    ofstream outputFile(output_circuit_path + "_new.v");
    if (!inputFile || !outputFile) {
        cerr << "Error opening files." << endl;
        exit(1);
    }
    int no_input = 1;
    string line;
    bool is_output = false;
    bool is_input = false;
    while(getline(inputFile, line)){
        
        istringstream iss(line);
        string check_if_gate;
        iss >> check_if_gate;
        if(check_if_gate == "input"){
            is_input = true;
            outputFile << line;
            for (char c : line) {
                if (c == ',') {
                     no_input++;
                }
            }
            while(getline(inputFile, line)){
                istringstream iss(line);
                string check_if_gate;
                iss >> check_if_gate;
                if(check_if_gate == "output"){
                    is_input = false;
                    is_output = true;
                    outputFile << endl;
                    outputFile << line;
                    break;
                }
                else{
                    outputFile << line;
                    for (char c : line) {
                        if (c == ',') {
                            no_input++;
                        }
                    }
                }
            }
        }
        if(is_input == true) outputFile << endl;
        if(check_if_gate == "and" || check_if_gate == "or" || check_if_gate == "nand" || check_if_gate == "nor" || check_if_gate == "xor" || check_if_gate == "xnor" || check_if_gate == "buf" || check_if_gate == "xnor" || check_if_gate == "not"){
            outputFile << removeSecondWord(line);
        }
        else{
            if(is_output){
                outputFile << endl;
                is_output = false;
                continue;
            }
            outputFile << line;
        }
        outputFile << "\n";
    }
    inputFile.close();
    outputFile.close();
    cout << "no of PIs in " << intput_circuit_path << " is " << no_input << endl << endl;
    return;
}

void read_strash_aig_cnf(Abc_Frame_t * pAbc, string name_of_circuit){
    int status;
    cout << "*********************************************" << endl;
    cout << "read strash aig cnf" << endl << endl;

    cout << "reading verilog: " << name_of_circuit << endl; 
    string command = "read_verilog " + name_of_circuit + "_new.v";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'read_verilog'.\n");
        exit(0);
    }
    cout << "verilog is read: " << name_of_circuit << endl;

    cout << endl << "printing stats of: " << name_of_circuit << " before strashing" << endl;
    status = Cmd_CommandExecute(pAbc, "print_stats");
    if (status) {
        fprintf(stderr, "Cannot execute the command 'print_stats'.\n");
        exit(0);
    }
    cout << endl;
    
    cout << "strash; balance; rewrite; refactor; balance; rewrite; balance; rewrite -z; balance;: " << name_of_circuit << endl; 
    command = "strash; balance; rewrite; refactor; balance; rewrite; balance; rewrite -z; balance;";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'strash'.\n");
        exit(0);
    }
    cout << "circuit simplification done:" << name_of_circuit << endl; 

    cout << endl << "printing stats of: " << name_of_circuit << " after strashing" << endl;
    status = Cmd_CommandExecute(pAbc, "print_stats");
    if (status) {
        fprintf(stderr, "Cannot execute the command 'print_stats'.\n");
        exit(0);
    }
    cout << endl;

    cout << "constructing AIG of: " << name_of_circuit << endl; 
    command = "write_aiger " + name_of_circuit + "_aig.aig";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'write_aiger'.\n");
        exit(0);
    }
    cout << "AIG is constructed: " << name_of_circuit << endl << endl;

    cout << "reading verilog: " << name_of_circuit << endl; 
    command = "read_verilog " + name_of_circuit + "_new.v";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'read_verilog'.\n");
        exit(0);
    }
    cout << "verilog is read: " << name_of_circuit << endl;

    cout << "constructing CNF of: " << name_of_circuit << endl; 
    command = "write_cnf "  + name_of_circuit + "_cnf.cnf";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'write_cnf'.\n");
        exit(0);
    }
    cout << "CNF is constructed: " << name_of_circuit << endl << endl; 

    cout << "constructing simplied verilog of: " << name_of_circuit << endl; 
    command = "write_verilog " + name_of_circuit + "_simplified.v";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'write_verilog'.\n");
        exit(0);
    }
    cout << "Verilog file is saved: " << name_of_circuit << endl << endl; 
    
    cout << "*********************************************" << endl << endl;
}