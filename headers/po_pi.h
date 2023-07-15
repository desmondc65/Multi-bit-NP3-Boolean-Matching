
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
#include <set>

#ifndef po_class_general_h
#define po_class_general_h
#endif

using namespace std;

class Primary_Output{
    protected:
    //basic info
    string po_name;
    bool negation;
    int wire_size;
    int support_size;

    //new verilog file info
    string file_name;
    vector<string> assigns;
    vector<string> pi;

    //for writing new verilog
    unordered_set<string> pi_new;
    unordered_set<string> wires;
    unordered_set<string> wires_new;

    //pi configurations
    vector<int> pi_negation_config;
    vector<int> pi_permutation_config;

public:
    
    //constructor 
    Primary_Output(string name) : po_name(name){
    }

    //get PO name
    string name() const{
        return this->po_name;
    }

    //add wire
    void add_wire(string wire){
        this->wires.emplace(wire);
    }

    //add pi
    void add_pi(string pi){
        this->pi.emplace_back(pi);
    }

    //set support size
    void set_support_size(){
        this->support_size = pi.size();
    }

    //set wire size
    void set_wire_size(){
        this->wire_size = wires.size();
    }

    //return support set size
    int pi_size(){
        return this->support_size;
    }

    //overwrite operator ==
    bool operator==(const Primary_Output& other) const {
        return po_name == other.po_name;
    }

    //set negation status false = +, true = -
    void negate(bool neg){
        this->negation = neg;
    }

    //return negate status of po
    bool po_negation(){
        if(this->negation){
            return true;
        }
        return false;
    }
    
    //return if it has wire
    bool wire_empty(){
        return this->wires.empty();
    }

    //return array of pi
    vector<string> get_pi(){
        return this->pi;
    }

    //check if this po contains this wire
    bool hasWire(const string& wire) const {
        return wires.find(wire) != wires.end();
    }

    //add assign line to this po
    void add_assign(const string line) {
        this->assigns.push_back(line);
    }

    //return bool status of no-th pi
    bool pi_negation(int no){
        if(pi_negation_config[no]){
            return true;
        }
        return false;
    }

    //set current negation config
    void set_pi_negate_config(vector<int>& neg_info){
        if(neg_info.size() != this->support_size){
            cout << "set_pi_negate wrong vector size" << endl;
            exit(0);
        }
        this->pi_negation_config = neg_info;
    }

    //add wires and pi to new wires
    void set_new_wires() {
        wires_new.clear();  // clear the current set to avoid any duplication

        // add primary inputs to new wires set
        for (const auto& pi_name : pi) {
            wires_new.insert(pi_name);
        }

        // add wires to new wires set
        for (const auto& wire_name : wires) {
            wires_new.insert(wire_name);
        }
    }

    void removeDuplicateInputs() {
        unordered_set<string> uniqueInputs;
        vector<string> uniqueInputList;

        for (const auto& input : pi) {
            if (uniqueInputs.insert(input).second) {
                uniqueInputList.push_back(input);
            }
        }

        pi = std::move(uniqueInputList);
    }

    //write po info into txt file
    void write_po_info(const string& filename) const {
        ofstream outputFile(filename);
        if (!outputFile) {
            cerr << "Error opening the file: " << filename << endl;
            return;
        }

        outputFile << "Name: " << po_name << endl;
        outputFile << "Support Size: " << support_size << endl;
        outputFile << "pi:" << endl;
        for (const auto& pi : pi) {
            outputFile << pi << endl;
        }
        outputFile << endl << "Wires:" << endl;
        for (const auto& wire : wires) {
            outputFile << wire << endl;
        }
        outputFile.close();
    }

    //write verilog of po
    void write_verilog(const string& filename) const{
        ofstream outputFile(filename);
        if (!outputFile) {
            cerr << "Error opening the file: " << filename << endl;
            return;
        }

        outputFile << "module top ( ";
        for(const auto& it: pi){
            outputFile << it << " , ";
        }
        outputFile << this->name() << " );" << endl;

        outputFile << "input  ";
        for(auto it = pi.begin(); it != pi.end(); ++it){
            outputFile << *it;
            if (it != prev(pi.end())) {
                outputFile << " , ";
            }
        }
        outputFile << " ;" << endl;

        outputFile << "output  ";
        outputFile << name() << " ;" << endl;

        if (!wires.empty()) {
            int counter = 0;
            int wire_size = wires.size();
            outputFile << "wire ";
            for (auto it = wires.begin(); it != wires.end(); ++it) {
                outputFile << *it;
                if (counter != wire_size - 1) {
                    outputFile << " , ";
                } else {
                    outputFile << ";";
                }
                counter++;
            }
            outputFile << endl;
        }
        
        for (auto it = assigns.rbegin(); it != assigns.rend(); ++it) {
            outputFile << *it << endl;
        }
        outputFile << "endmodule" << endl;
        outputFile.close();
    }
};

bool compareBySupportSize( Primary_Output& a,  Primary_Output& b) {
    return a.pi_size() < b.pi_size();
}


//circuit 1 po class
class c1_PO: public Primary_Output{
    //for the same naming during verification
    vector<string> pi_test_name;

public:
    //constructor 
    c1_PO(string name) : Primary_Output(name){
    }

    //set new names for verilog
    void set_pi_test_config(){
        pi_test_name.resize(this->pi_size());
        for(int i = 0; i < this->pi_size(); i++){
            pi_test_name[i] = "pi_" + to_string( i+1 );
            pi_new.emplace(pi_test_name[i]);
        }
    }

    //return the no-th pi's name
    string pi_name(int no){
        return this->pi[no];
    }

    //write new verilog file of po for testing
    void write_test_verilog(const string& filename){

        ofstream outputFile(filename);
        file_name = filename;

        //error checker
        if (!outputFile) {
            cerr << "Error opening the file: " << filename << endl;
            return;
        }

        //module line
        outputFile << "module top ( ";
        if(this->pi_new.empty()){
            cout << "inputs_new_name is empty!" << endl;
            exit(0);
        }
        if(wires_new.empty()){
            cout << this->name() << " new_wires is empty!" << endl;
            exit(0);
        }
        for(const auto& it: pi_new){
            outputFile << it << " , ";
        }
        outputFile << "po" << " );" << endl;
        
        //input line
        outputFile << "input  ";
        for (auto it = pi_new.begin(); it != pi_new.end(); ++it) {
            outputFile << *it;
            if (next(it) != pi_new.end()) { // If not the last iteration
                outputFile << " , ";
            }
        }
        outputFile << " ;" << endl;

        //outputline
        outputFile << "output  " << "po" << " ;"  << endl;
        
        //wire line
        outputFile << "wire ";
        for (auto it = wires_new.begin(); it != wires_new.end(); ++it) {
            outputFile << *it << " , ";
        }
        outputFile << this->name() << " ;"  << endl;
        outputFile << endl;
        
        //assign new names to original pi
        for(int i = 0; i < support_size; i++){
            if(pi_negation_config[i] == 1){
                outputFile << "assign " << pi[i] << " = " << "~" + pi_test_name[i] << " ;";
            } 
            else{
                outputFile << "assign " << pi[i] << " = " << pi_test_name[i] << " ;";
            }
            outputFile << endl;
        }
        for (auto it = assigns.rbegin(); it != assigns.rend(); ++it) {
            outputFile << *it << endl;
        }
        //assign new names to original po
        if(this->negation){
            outputFile << "assign " << "po = ~" + this->po_name << " ;" << endl;  
        }
        else{
            outputFile << "assign " << "po = " + this->po_name << " ;" << endl;  
        }
        outputFile << "endmodule" << endl;

        outputFile.flush();
        outputFile.close();
    }
};

//circuit 2 po class
class c2_PO: public Primary_Output{
    //for the same naming during verification
    vector<string> pi_test_name;
public:
    //constructor 
    c2_PO(string name) : Primary_Output(name){
    }
    //set c2 po's pi permutation configurations
    void set_c2_pi_permute_config(vector<int>& vec){
        if(vec.size() != this->support_size){
            cout <<" set_c2_pi_permute error " << endl;
            exit(0);
        }
        this->pi_permutation_config = vec;
    }

    //set new names for verilog
    void set_pi_test_config(){
        pi_test_name.resize(this->pi_size());
        for(int i = 0; i < this->pi_size(); i++){
            pi_test_name[i] = "pi_" + to_string(pi_permutation_config[i]);
            pi_new.emplace(pi_test_name[i]);
        }
    }

    //return the no-th pi's name
    string pi_name(int no){
        return this->pi[no];
    }

    //return the no-th value of c2 pi permutation config
    int pi_no(int no){
        return pi_permutation_config[no] - 1;
    }
    
    //write new verilog file of po for testing
    void write_test_verilog(const string& filename){

        ofstream outputFile(filename);
        file_name = filename;

        //error checker
        if (!outputFile) {
            cerr << "Error opening the file: " << filename << endl;
            return;
        }

        //module line
        outputFile << "module top ( ";
        if(this->pi_new.empty()){
            cout << "inputs_new_name is empty!" << endl;
            exit(0);
        }
        if(wires_new.empty()){
            cout << this->name() << " new_wires is empty!" << endl;
            exit(0);
        }
        for(const auto& it: pi_new){
            outputFile << it << " , ";
        }
        outputFile << "po" << " );" << endl;
        
        //input line
        outputFile << "input  ";
        for (auto it = pi_new.begin(); it != pi_new.end(); ++it) {
            outputFile << *it;
            if (next(it) != pi_new.end()) { // If not the last iteration
                outputFile << " , ";
            }
        }
        outputFile << " ;" << endl;

        //outputline
        outputFile << "output  " << "po" << " ;"  << endl;
        
        //wire line
        outputFile << "wire ";
        for (auto it = wires_new.begin(); it != wires_new.end(); ++it) {
            outputFile << *it << " , ";
        }
        outputFile << this->name() << " ;"  << endl;
        outputFile << endl;
        
        //assign new names to original pi
        for(int i = 0; i < support_size; i++){
            if(pi_negation_config[i] == 1){
                outputFile << "assign " << pi[i] << " = " << "~" + pi_test_name[i] << " ;";
            } 
            else{
                outputFile << "assign " << pi[i] << " = " << pi_test_name[i] << " ;";
            }
            outputFile << endl;
        }
        for (auto it = assigns.rbegin(); it != assigns.rend(); ++it) {
            outputFile << *it << endl;
        }
        //assign new names to original po
        if(this->negation){
            outputFile << "assign " << "po = ~" + this->po_name << " ;" << endl;  
        }
        else{
            outputFile << "assign " << "po = " + this->po_name << " ;" << endl;  
        }
        outputFile << "endmodule" << endl;
        
        outputFile.flush();
        outputFile.close();
    }
};

void extract_wires_pi1(string line, vector<c1_PO> &circuit__po_self);
void extract_wires_pi2(string line, vector<c2_PO> &circuit__po_self);

//separate pis and commands of each po
void write_pi_po_assigns(vector<string>& pi, vector<string>& po, vector<string>& assign, string folder, string cir_name){
    string path = folder + "/" + cir_name + "_pi";
    ofstream outputfile(path);
    outputfile << "This file contains the PIs of " + cir_name << endl;
    for(auto i : pi){
        outputfile << i << endl;
    }
    outputfile.close();

    path = folder + "/" + cir_name + "_po";
    ofstream outputfile2(path);
    outputfile2 << "This file contains the POs of " + cir_name << endl;
    for(auto i : po){
        outputfile2 << i << endl;
    }
    outputfile2.close();

    path = folder + "/" + cir_name + "_assigns";
    ofstream outputfile3(path);
    outputfile3 << "This file contains the assigns of " + cir_name << endl;
    for(auto i : assign){
        outputfile3 << i << endl;
    }
    outputfile3.close();
}

void sort_assigns_to_po1(vector<c1_PO> &circuit_po_self, vector<string>& vec_po, vector<string>& circuit_verilog_assigns, string& folder, string& circ){
    cout << "*********************************************" << endl << endl;
    cout << "sort_assigns_to_po 1:" + circ << endl;
    cout << "here" << endl;
    for(auto &it: vec_po){
        circuit_po_self.emplace_back(c1_PO(it));
    }
    cout << "here" << endl;
    int assigns_size = circuit_verilog_assigns.size();
    int counter = 0;
    for(auto it = circuit_verilog_assigns.rbegin(); it != circuit_verilog_assigns.rend(); ++it){
        cout << counter++ << endl;
        string assign_line = *it;
        cout << assign_line << endl;
        extract_wires_pi1(assign_line, circuit_po_self);
        
    }
    cout << "wire extracted" << endl;
    for (auto& p : circuit_po_self) {
        p.removeDuplicateInputs();
        p.set_support_size();
    }

    //create po folder
    string folderName =   folder + "_po/";
    int folderCreated = mkdir(folderName.c_str(), 0777);

    for (const auto& p : circuit_po_self){
        p.write_po_info(folderName + p.name());
        p.write_verilog(folderName + p.name() + ".v");
        cout << folderName + p.name() << endl;
        
    }
    
    cout << "line process done" << endl<< endl;
    cout << "*********************************************" << endl << endl;
}

void sort_assigns_to_po2(vector<c2_PO> &circuit_po_self, vector<string>& vec_po, vector<string>& circuit_verilog_assigns, string& folder, string& circ){
    cout << "*********************************************" << endl << endl;
    cout << "sort_assigns_to_po :" + circ << endl;
    cout << "here" << endl;
    for(auto &it: vec_po){
        circuit_po_self.emplace_back(c2_PO(it));
    }
    cout << "here" << endl;
    int assigns_size = circuit_verilog_assigns.size();
    for(auto it = circuit_verilog_assigns.rbegin(); it != circuit_verilog_assigns.rend(); ++it){
        string assign_line = *it;
        extract_wires_pi2(assign_line, circuit_po_self);
    }

    for (auto& p : circuit_po_self) {
        p.removeDuplicateInputs();
        p.set_support_size();
    }

    //create po folder
    string folderName =   folder + "_po/";
    int folderCreated = mkdir(folderName.c_str(), 0777);

    for (const auto& p : circuit_po_self){
        p.write_po_info(folderName + p.name());
        p.write_verilog(folderName + p.name() + ".v");
        cout << folderName + p.name() << endl;
        
    }
    
    cout << "line process done" << endl<< endl;
    cout << "*********************************************" << endl << endl;
}

void extract_wires_pi1(string line, vector<c1_PO> &circuit__po_self){
    istringstream iss(line);
    string assign_piece; //assign n44 = ~new_n66_ & ~new_n67_;
    iss >> assign_piece; //assign
    string left_node;
    iss >> left_node; // n44
    string equal_node;
    iss >> equal_node;
    string right_node_1;
    iss >> right_node_1;
    if(right_node_1[0] == '~'){
        right_node_1 = right_node_1.substr(1);
    }
    string operand;
    iss >> operand;
    string right_node_2;
    iss >> right_node_2;
    right_node_2.pop_back();
    if(right_node_2[0] == '~'){
        right_node_2 = right_node_2.substr(1);
    }
    cout << "here 1" << endl;
    if(left_node[1] == 'e'){ //wire
        bool found_wire = false;
        vector<c1_PO>::iterator it = circuit__po_self.begin();
        for(;it != circuit__po_self.end(); it++){
            if(it->hasWire(left_node)){
                found_wire = true;
                c1_PO& found_po = const_cast<c1_PO&>(*it);
                found_po.add_assign(line);
                if(right_node_1[1] == 'e'){ //if wire
                    found_po.add_wire(right_node_1);
                }
                else{ // if pi
                    found_po.add_pi(right_node_1);
                }
                if(right_node_2[1] == 'e'){ //if wire
                    found_po.add_wire(right_node_2);
                }
                else{ // if pi
                    found_po.add_pi(right_node_2);
                }
                continue;
            }
        }
        if(found_wire == false && it == circuit__po_self.end()){
            cout << "cant find wire match during extraction" << endl;
            exit(0);
        }
    }
    else{ //po
        vector<c1_PO>::iterator it = circuit__po_self.begin();
        for(;it != circuit__po_self.end(); it++){
            if(it->name() == left_node){
                break;
            }
        }
        if(it == circuit__po_self.end()){
            cout << "cant find po during extraction" << endl;
            exit(0);
        }
        cout << "extract donesf1 " << endl;
        c1_PO& found_po = const_cast<c1_PO&>(*it);
        found_po.add_assign(line);
         cout << "extract done1 342" << endl;
        if(right_node_1[1] == 'e'){ //if wire
            found_po.add_wire(right_node_1);
            cout << "extract done1 342" << endl;
        }
        else{ // if pi
        cout << "extract done1 3fg2" << endl;
            found_po.add_pi(right_node_1);
            cout << "extract done1 3dfg42" << endl;
        }
        if(right_node_2[1] == 'e'){ //if wire
            found_po.add_wire(right_node_2);
            //cout << "added: " << right_node_2 << endl;
        }
        else{ // if pi
        cout << "extract done1 sdfsd342" << endl;
            found_po.add_pi(right_node_2);
            cout << "extracsdfsdt done1 sdfsd342" << endl;
        }
         cout << "extract done12 " << endl;
    }
    cout << "extract done" << endl;
    //cout << "*********************************************" << endl << endl;
}

void extract_wires_pi2(string line, vector<c2_PO> &circuit__po_self){
    istringstream iss(line);
    string assign_piece; //assign n44 = ~new_n66_ & ~new_n67_;
    iss >> assign_piece; //assign
    string left_node;
    iss >> left_node; // n44
    string equal_node;
    iss >> equal_node;
    string right_node_1;
    iss >> right_node_1;
    if(right_node_1[0] == '~'){
        right_node_1 = right_node_1.substr(1);
    }
    string operand;
    iss >> operand;
    string right_node_2;
    iss >> right_node_2;
    right_node_2.pop_back();
    if(right_node_2[0] == '~'){
        right_node_2 = right_node_2.substr(1);
    }

    if(left_node[1] == 'e'){ //wire
        bool found_wire = false;
        vector<c2_PO>::iterator it = circuit__po_self.begin();
        for(;it != circuit__po_self.end(); it++){
            if(it->hasWire(left_node)){
                found_wire = true;
                c2_PO& found_po = const_cast<c2_PO&>(*it);
                found_po.add_assign(line);
                if(right_node_1[1] == 'e'){ //if wire
                    found_po.add_wire(right_node_1);
                }
                else{ // if pi
                    found_po.add_pi(right_node_1);
                }
                if(right_node_2[1] == 'e'){ //if wire
                    found_po.add_wire(right_node_2);
                }
                else{ // if pi
                    found_po.add_pi(right_node_2);
                }
                continue;
            }
        }
        if(found_wire == false && it == circuit__po_self.end()){
            cout << "cant find wire match during extraction" << endl;
            exit(0);
        }
    }
    else{ //po
        vector<c2_PO>::iterator it = circuit__po_self.begin();
        for(;it != circuit__po_self.end(); it++){
            if(it->name() == left_node){
                break;
            }
        }
        if(it == circuit__po_self.end()){
            cout << "cant find po during extraction" << endl;
            exit(0);
        }
        c2_PO& found_po = const_cast<c2_PO&>(*it);
        found_po.add_assign(line);
        if(right_node_1[1] == 'e'){ //if wire
            found_po.add_wire(right_node_1);
        }
        else{ // if pi
            found_po.add_pi(right_node_1);
        }
        if(right_node_2[1] == 'e'){ //if wire
            found_po.add_wire(right_node_2);
            //cout << "added: " << right_node_2 << endl;
        }
        else{ // if pi
            found_po.add_pi(right_node_2);
        }
    }
    //cout << "*********************************************" << endl << endl;
}

void print_po_self(vector<c1_PO>& circ, string cir_name){
    cout << cir_name << ": " << endl;
    for (auto& poObject : circ) {
        cout << "Name: " << poObject.name() << endl;
        cout << "Support Size: " << poObject.pi_size() << endl;
        // You can print other details of the 'po' object if needed
        cout << endl;
    }
}

void print_po_self(vector<c2_PO>& circ, string cir_name){
    cout << cir_name << ": " << endl;
    for (auto& poObject : circ) {
        cout << "Name: " << poObject.name() << endl;
        cout << "Support Size: " << poObject.pi_size() << endl;
        // You can print other details of the 'po' object if needed
        cout << endl;
    }
}

void setNewWiresForAll(vector<c1_PO>& circuit_c1_pos) {
    for (auto& poObject : circuit_c1_pos) {
        poObject.set_new_wires();
    }
}

void setNewWiresForAll(vector<c2_PO>& circuit_c2_pos) {
    for (auto& poObject : circuit_c2_pos) {
        poObject.set_new_wires();
    }
}