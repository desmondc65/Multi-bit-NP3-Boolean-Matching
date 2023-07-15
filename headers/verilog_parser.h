#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <utility>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef verilog_parser_h
#define verilog_parser_h
#endif

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

void verilog_parser(string intput_circuit_path, vector<string>& verilog_assigns, vector<string>& vec_pis, vector<string>& vec_pos, string cir_name){
    ifstream inputFile(intput_circuit_path);
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
        get_pi(line, vec_pis);
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
        get_pi(line, vec_pos);
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
        
        verilog_assigns.emplace_back(line);
        getline(inputFile, line);
        istringstream iss(line);
        iss >> check_first_element;
    }
    //print_vector_string(verilog_assigns);

    string folderName =   cir_name + "_data/";
    int folderCreated = mkdir(folderName.c_str(), 0777);
    string old_path = cir_name + "_aig.aig";
    string new_path = folderName + cir_name + "_aig.aig";
    rename( old_path.c_str() , new_path.c_str());

    old_path = cir_name + "_cnf.cnf";
    new_path = folderName + cir_name + "_cnf.cnf";
    rename( old_path.c_str() , new_path.c_str());

    old_path = cir_name + "_simplified.v";
    new_path = folderName + cir_name +"_simplified.v";
    rename( old_path.c_str() , new_path.c_str());

    old_path = cir_name + "_new.v";
    new_path = folderName + cir_name +"_new.v";
    rename( old_path.c_str() , new_path.c_str());
}

