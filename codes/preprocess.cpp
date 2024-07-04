#include "preprocess.h"

void read_input(string& input_name, circuit_info& c1, circuit_info& c2){

    ifstream infile;
    infile.open(input_name);
    if(!infile){
        std::cout << "Unable to open file" << endl;
        exit(1);
    }
    
    //read circ 1 info
    infile >> c1.path_v;
    infile >> c1.no_of_pis;
    string line;
    c1.bus_size_input.resize(c1.no_of_pis);
    c1.bus_input.resize(c1.no_of_pis);
    getline(infile, line);
    for(int i = 0; i < c1.no_of_pis; i++){
        getline(infile, line);
        istringstream iss(line);
        iss >> c1.bus_size_input[i] >> c1.bus_input[i].first >> c1.bus_input[i].second;
        //cout << c1_bus_size_input[i] << c1_bus_input_1[i] << c1_bus_input_2[i];
        //cout << line << endl;
    }

    //read circ 2 info
    infile >> c2.path_v;
    infile >> c2.no_of_pis;
    c2.bus_input.resize(c2.no_of_pis);
    c2.bus_size_input.resize(c2.no_of_pis);
    getline(infile, line);
    for(int i = 0; i < c2.no_of_pis; i++){
        getline(infile, line);
        istringstream iss(line);
        iss >> c2.bus_size_input[i] >> c2.bus_input[i].first >> c2.bus_input[i].second;
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

void prepare_input(circuit_info& cir){
    ifstream inputFile(cir.path_v);
    ofstream outputFile(cir.name + "_new.v");
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
    cout << "no of PIs in " << cir.name << " is " << no_input << endl << endl;
    return;
}

void read_strash_aig_cnf(Abc_Frame_t * pAbc, circuit_info& cir){
    int status;
    cout << "*********************************************" << endl;
    cout << "read strash aig cnf" << endl << endl;

    cout << "reading verilog: " << cir.name << endl; 
    string command = "read_verilog " + cir.name + "_new.v";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'read_verilog'.\n");
        exit(0);
    }
    cout << "verilog is read: " << cir.name << endl;

    cout << endl << "printing stats of: " << cir.name << " before strashing" << endl;
    status = Cmd_CommandExecute(pAbc, "print_stats");
    if (status) {
        fprintf(stderr, "Cannot execute the command 'print_stats'.\n");
        exit(0);
    }
    cout << endl;

    status = Cmd_CommandExecute(pAbc, "print_supp");
    if (status) {
        fprintf(stderr, "Cannot execute the command 'print_supp'.\n");
        exit(0);
    }
    cout << endl;
    
    cout << "strash; balance; rewrite; refactor; balance; rewrite; balance; rewrite -z; balance;: " << cir.new_path << endl; 
    command = "strash; balance; rewrite; refactor; balance; rewrite; balance; rewrite -z; balance;";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'strash'.\n");
        exit(0);
    }
    cout << "circuit simplification done:" << cir.name << endl; 

    cout << endl << "printing stats of: " << cir.name << " after strashing" << endl;
    status = Cmd_CommandExecute(pAbc, "print_stats");
    if (status) {
        fprintf(stderr, "Cannot execute the command 'print_stats'.\n");
        exit(0);
    }
    cout << endl;

    cout << "constructing AIG of: " << cir.name << endl; 
    command = "write_aiger " + cir.name + "_aig.aig";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'write_aiger'.\n");
        exit(0);
    }
    cout << "AIG is constructed: " << cir.name << endl << endl;

    cout << "constructing CNF of: " << cir.name << endl; 
    command = "write_cnf "  + cir.name + "_cnf.cnf";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'write_cnf'.\n");
        exit(0);
    }
    
    cout << "CNF is constructed: " << cir.name << endl << endl; 

    cout << "constructing simplied verilog of: " << cir.name << endl; 
    command = "write_verilog " + cir.name + "_simplified.v";
    status = Cmd_CommandExecute(pAbc, command.c_str());
    if (status) {
        fprintf(stderr, "Cannot execute the command 'write_verilog'.\n");
        exit(0);
    }
    cout << "Verilog file is saved: " << cir.name << endl << endl; 
 
    string folderName =   cir.name + "_data/";
    cir.folder_name = folderName;
    int folderCreated = mkdir(folderName.c_str(), 0777);
    string old_path = cir.name + "_aig.aig";
    string new_path = folderName + cir.name + "_aig.aig";
    cir.aig_path = new_path;
    rename( old_path.c_str() , new_path.c_str());

    old_path = cir.name + "_cnf.cnf";
    new_path = folderName + cir.name + "_cnf_abc.cnf";
    cir.cnf_path = new_path;
    rename( old_path.c_str() , new_path.c_str());

    old_path = cir.name + "_simplified.v";
    new_path = folderName + cir.name +"_simplified.v";
    cir.simplified_v = new_path;
    rename( old_path.c_str() , new_path.c_str());

    old_path = cir.name + "_new.v";
    new_path = folderName + cir.name +"_new.v";
    cir.new_path = new_path;
    rename( old_path.c_str() , new_path.c_str());
    
    cout << "*********************************************" << endl << endl;
}
