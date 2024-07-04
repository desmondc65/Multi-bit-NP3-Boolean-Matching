#include "verilog_parser.h"

using namespace std;

void get_pi(string line, vector<string>& vec_pis){
    stringstream ss(line);
    string word;
    while (ss >> word) {
        // Remove leading or trailing spaces and commas
        word.erase(0, word.find_first_not_of(" ,"));
        word.erase(word.find_last_not_of(" ,") + 1);
        word.erase(word.find_last_not_of(";") + 1);
        // Ignore the "input" keyword and the semicolon at the end
        if (word != "input" && word != "output") {
            vec_pis.push_back(word);
        }
        
    }
}

void verilog_parser(circuit_info& cir){
    ifstream inputFile(cir.simplified_v);
    string line;
    string check_first_element;
    
    //get the first two redundant lines
    getline(inputFile, line);
    getline(inputFile, line);

    //get the redundant module 
    while(getline(inputFile, line)){
        istringstream iss(line);
        iss >> check_first_element;
        if(check_first_element == "input"){
            break;
        }
    }
    //extract pis
    while(check_first_element != "output"){
        get_pi(line, cir.pis_name);
        //cout << line << endl;
        getline(inputFile, line);
        istringstream iss(line);
        iss >> check_first_element;
        //cout << "here " << vec_pis.size() << endl;
    }
    //print_pi(vec_pis, intput_circuit_path);
    //cout << intput_circuit_path << " has "<< vec_pis.size() << " pis" << endl << endl;

    //extract pos
    while(check_first_element != "wire"){
        get_pi(line, cir.pos_name);
        getline(inputFile, line);
        istringstream iss(line);
        iss >> check_first_element;
    }
    //cout << intput_circuit_path << " has "<< vec_pos.size() << " pos" << endl << endl;
    //print_pi(vec_pos, intput_circuit_path);

    //parse to assign
    while(check_first_element != "assign"){
        getline(inputFile, line);
        istringstream iss(line);
        iss >> check_first_element;
    }

    //get all lines
    while(check_first_element == "assign"){
        
        cir.assigns.emplace_back(line);
        getline(inputFile, line);
        istringstream iss(line);
        iss >> check_first_element;
    }
    cir.no_of_pis = cir.pis_name.size();
    cir.no_of_pos = cir.pos_name.size();
    cout << "verilog of " << cir.new_path << " parsed" << endl << endl;
}

//separate pis and commands of each po
void write_pi_po_assigns(circuit_info& cir){
    cout << "writing pi po and assigns of " << cir.name << endl << endl;
    string path = cir.folder_name + cir.name  + "_pi";
    ofstream outputfile(path);
    outputfile << "This file contains the PIs of " + cir.name << endl;
    for(auto i : cir.pis_name){
        outputfile << i << endl;
    }
    outputfile.close();

    path = cir.folder_name + cir.name + "_po";
    ofstream outputfile2(path);
    outputfile2 << "This file contains the POs of " + cir.name << endl;
    for(auto i : cir.pos_name){
        outputfile2 << i << endl;
    }
    outputfile2.close();

    path = cir.folder_name + cir.name + "_assigns";
    ofstream outputfile3(path);
    outputfile3 << "This file contains the assigns of " + cir.name << endl;
    for(auto i : cir.assigns){
        outputfile3 << i << endl;
    }
    outputfile3.close();
}