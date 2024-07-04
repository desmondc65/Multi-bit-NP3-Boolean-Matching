#include "circuit.h"
#define debug false

using namespace std;

circuit_info::circuit_info(string cir){
    this->name = cir;
    this->folder_name = cir + "_data/";
}

void circuit_info:: cir_print(){
        cout << "path_v: " << path_v << endl;
        cout << "new_path: " << new_path << endl;
        cout << "folder name: " << folder_name << endl;
        cout << "simplified_v: " << simplified_v << endl;
}

void circuit_info::verilog_to_cnf(){
    //add pis, pos to literals
    for(auto const &i : pis_name){
        all_literals.emplace(i, literal_no);
        literals.emplace(i, literal_no++);
    }

    for(auto  &i : pos){
        i.var.emplace(i.po_name);
        i.literal_no = literal_no;
        all_literals.emplace(i.po_name, literal_no);
        literals.emplace(i.po_name, literal_no++);
    }

    //get wires
    ifstream file(this->simplified_v);
    string line;
    string first_word;

    //traverse to wire
    while(getline(file, line)){
        istringstream iss(line);
        iss >> first_word;
        if(first_word == "wire"){
            break;
        }
    }

    //get wire names
    while(first_word != "assign"){
        get_wire(line);
        getline(file, line);
        istringstream iss(line);
        iss >> first_word;
    }

    if(debug)this->print_wire();

    //add wires to literals
    for(auto const &i : wire){
        all_literals.emplace(i + this->name, literal_no);
        literals.emplace(i, literal_no++);
    }
    vector<string> assigns;

    while(first_word == "assign"){
        istringstream iss(line);
        assigns.emplace_back(line);
        getline(file, line);
        iss >> first_word;
        if(line == "endmodule") break;
    }

    reverse(assigns.begin(), assigns.end());
    
    //parse assign lines from behind
    for(auto &line_assign: assigns){
        istringstream iss(line_assign);
        string left, equal, op, r1, r2; //assign u0 = x0 & y0;
        int num_words = distance(istream_iterator<string>{iss}, istream_iterator<string>());

        bool is_r1_negative = false;
        bool is_r2_negative = false;
        int left_literal_no, r1_literal_no, r2_literal_no;

        //assign h0 = a0
        if(num_words == 4){
            iss >> first_word >> left >> equal >> r1;
            r1.pop_back(); // pop ";"

            if(debug) cout << left << " " << equal << " " << r1;
            if(debug) cout << left << endl;
            auto it = literals.find(left);
            if(it != literals.end()){
                left_literal_no = it->second;
            }
            else{
                cout << "error: left literal can't be found!" << endl;
                exit(0);
            }

            if(r1[0] == '~'){
                is_r1_negative = true;
                r1.erase(r1.begin());
            }

            it = literals.find(r1);
            if(it != literals.end()){
                r1_literal_no = it->second;
            }
            else{
                cout << "error: right 1 literal can't be found!" << endl;
                exit(0); 
            }

            //tseitin
            // x = -p, cnf = (-x | -p) & (p | x)
            // x = p, cnf = (-x | p) & (-p | x)
            if(is_r1_negative == true){ //negative a0
                clauses.emplace_back(vector<int>{-left_literal_no, -r1_literal_no, 0}); // (-x | -p)
                clauses.emplace_back(vector<int>{left_literal_no, r1_literal_no, 0}); // (p | x)
            }
            else if(is_r1_negative == false){
                clauses.emplace_back(vector<int>{-left_literal_no, r1_literal_no, 0}); // (-x | p)
                clauses.emplace_back(vector<int>{left_literal_no, -r1_literal_no, 0}); // (-p | x)
            }
            else{
                cout << "error: tseitin for one right operand" << endl;
                exit(0);
            }
            for(auto &i: pos){
                if(i.var.find(left) != i.var.end()){
                    if(is_r1_negative == true){ //negative a0
                        i.clauses.emplace_back(vector<int>{-left_literal_no, -r1_literal_no, 0}); // (-x | -p)
                        i.clauses.emplace_back(vector<int>{left_literal_no, r1_literal_no, 0}); // (p | x)
                    }
                    else if(is_r1_negative == false){
                        i.clauses.emplace_back(vector<int>{-left_literal_no, r1_literal_no, 0}); // (-x | p)
                        i.clauses.emplace_back(vector<int>{left_literal_no, -r1_literal_no, 0}); // (-p | x)
                    }
                    i.var.emplace(r1);
                }
            }
        }

        //assign h0 = a0 & b0        
        // x = p & q, cnf = (-x | p) & (-x | q) & (-p | -q | x)
        // x = p | q, cnf = (x | -p) & (x | -q) & (p | q | -x)
        else if(num_words == 6){
            istringstream isss(line_assign);
            isss >> first_word >> left >> equal >> r1 >> op >> r2; 
            
            if(debug) cout << "debug " <<first_word << " " << left << " " << equal << " " << r1 << " " << op << " " << r2 << endl;
            r2.pop_back();
            if(debug)cout << line << endl;

            auto it = literals.find(left);
            if(it != literals.end()){
                left_literal_no = it->second;
            }
            else{
                cout << "error in num words = 6a: left literal can't be found!" << endl;
                cout << line << endl;
                exit(0);
            }

            if(r1[0] == '~'){
                is_r1_negative = true;
                r1.erase(r1.begin());
            }

            if(r2[0] == '~'){
                is_r2_negative = true;
                r2.erase(r2.begin());
            }

            it = literals.find(r1);
            if(it != literals.end()){
                r1_literal_no = it->second;
            }
            else{
                cout << "error: right 1 literal can't be found!" << endl;
                exit(0); 
            }
            if (debug) cout << "right2 is: " << r2 << endl;
            it = literals.find(r2);
            if(it != literals.end()){
                r2_literal_no = it->second;
            }
            else{
                cout << "error: right 2 literal can't be found!" << endl;
                exit(0); 
            }

            // x = p & q, cnf = (-x | p) & (-x | q) & (-p | -q | x)
            if(op == "&"){
                clauses.emplace_back(vector<int>{-left_literal_no, (is_r1_negative? -r1_literal_no : r1_literal_no), 0}); // (-x | p)
                clauses.emplace_back(vector<int>{-left_literal_no, (is_r2_negative? -r2_literal_no : r2_literal_no), 0}); // (-x | q)
                clauses.emplace_back(vector<int>{left_literal_no, (is_r1_negative? r1_literal_no : -r1_literal_no), (is_r2_negative? r2_literal_no : -r2_literal_no), 0}); //(-p | -q | x)
            }
            // x = p | q, cnf = (x | -p) & (x | -q) & (p | q | -x)
            else if(op == "|"){
                clauses.emplace_back(vector<int>{left_literal_no, ((is_r1_negative)? r1_literal_no : -r1_literal_no), 0}); // (x | -p)
                clauses.emplace_back(vector<int>{left_literal_no, (is_r2_negative? r2_literal_no : -r2_literal_no), 0}); // (x | -q)
                clauses.emplace_back(vector<int>{-left_literal_no, (is_r1_negative? -r1_literal_no : r1_literal_no), (is_r2_negative? -r2_literal_no : r2_literal_no), 0}); //(p | q | -x)
            }
            else{
                cout << "error: wrong operand in verilog to cnf of " << this->name << endl;;
            }
            for(auto &i: pos){
                if(i.var.find(left) != i.var.end()){
                    i.var.emplace(r1);
                    i.var.emplace(r2);
                    if(op == "&"){
                        i.clauses.emplace_back(vector<int>{-left_literal_no, (is_r1_negative? -r1_literal_no : r1_literal_no), 0}); // (-x | p)
                        i.clauses.emplace_back(vector<int>{-left_literal_no, (is_r2_negative? -r2_literal_no : r2_literal_no), 0}); // (-x | q)
                        i.clauses.emplace_back(vector<int>{left_literal_no, (is_r1_negative? r1_literal_no : -r1_literal_no), (is_r2_negative? r2_literal_no : -r2_literal_no), 0}); //(-p | -q | x)
                    }
                    // x = p | q, cnf = (x | -p) & (x | -q) & (p | q | -x)
                    else if(op == "|"){
                        i.clauses.emplace_back(vector<int>{left_literal_no, ((is_r1_negative)? r1_literal_no : -r1_literal_no), 0}); // (x | -p)
                        i.clauses.emplace_back(vector<int>{left_literal_no, (is_r2_negative? r2_literal_no : -r2_literal_no), 0}); // (x | -q)
                        i.clauses.emplace_back(vector<int>{-left_literal_no, (is_r1_negative? -r1_literal_no : r1_literal_no), (is_r2_negative? -r2_literal_no : r2_literal_no), 0}); //(p | q | -x)
                    }
                    else{
                        cout << "error: wrong operand in verilog to cnf of " << this->name << endl;;
                    }
                }
            }

        }
        else{
            cout << "bug: wrong assign counts" << endl;
            exit(0);
        }
    }
    print_each_po_cnf();
    if(debug) cout << endl;
    cout << "verilog to cnf of " << this->name << " done" << endl << endl;
}

void circuit_info::print_each_po_cnf(){
    string path = folder_name + name + "_each_po_cnf";
    ofstream outputfile(path);
    for(auto &i: pos){
        outputfile << "po name: " << i.po_name << endl;
        for(auto &j: i.clauses){
            for(auto &k: j){
                outputfile << k << " ";
            }
            outputfile << endl;
        }
    }
}

void circuit_info::get_wire(string& line){
    stringstream ss(line);
    string word;
    while(ss >> word){
        if(word[0] == 'n'){
            if(word.back() == ',' || word.back() == ';') word.pop_back();
            wire.emplace_back(word);
        }
    }

}

void circuit_info::print_assign(){
    for(auto i : assigns){
        cout << i << endl;
    }
    cout << endl;
}

void circuit_info::print_wire(){
    for(auto i : wire){
        cout << i << endl;
    }
    cout << endl;
}

void circuit_info::dissect(){
    // cout << "dissecting " << name << endl << endl;
    // for(auto &i: pos){
    //     i.lits.emplace(i.literal_no);
    // }
    // for(auto it = clauses.rbegin(); it != clauses.rend(); ++it){
    //     for(auto po: pos){
    //         for(auto lit = it->begin(); lit != it->end(); lit++){
    //             if(*lit == 0) break;
    //             int check_lit =  *lit;
    //             if(check_lit < 0){
    //                 check_lit = -check_lit;
    //             }
    //             if(po.lits.fin)
    //         }
    //     }
    // }
    // cout << "dissecting completed" << name << endl << endl;
}

void circuit_info::print_clauses(){
    //write cnf info to file
    string path = folder_name + name + "_cnf_info";
    ofstream cir_cnf(path);
    cir_cnf << "this file contains the cnf info of " << name << endl;
    for(auto i: clauses){
        for(auto j: i){
            cir_cnf << j << " ";
        }
        cir_cnf << endl;
    }
    cir_cnf << endl;
    cir_cnf.close();
}

void circuit_info::write_literals_to_file(){
    string path = folder_name + name + "_literals_no";
    ofstream outputfile(path);
    outputfile << "this file constains the corresponding literals no of " << this->name << endl;
    for(auto i : literals){
        outputfile << i.first << " " << i.second << endl;
    }
    outputfile.close();
}

void circuit_info::sort_circuit_po(){
    // sort(vec.begin(), vec.end(), [](const po& a, const po& b) {
    //     return a.support_size < b.support_size;
    // });
    sort(pos.begin(), pos.end(), [](const po &a, const po &b){
        return a.support_size <= b.support_size;
    });

    // for(auto const &i: pos){
    //     cout << i.po_name << " " << i.support_size << endl;
    // }
}