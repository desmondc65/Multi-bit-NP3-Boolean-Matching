#include "constraints_generator.h"

void mi_info::mi_constraints_generator(const vector<vector<mi>>& input_mapping){
    cout << "generating constraint 3 of NP3 Boolean Matching: ensures every input of g should be mapped to either an input of f or a constant" << endl << endl;
    cout << "generating constraint 5 of NP3 Boolean Matching: MI mapping" << endl << endl;
    for(auto i = input_mapping.begin(); i != input_mapping.end(); ++i){
        //at least one constraint
        vector<int> all_abi;
        for(auto j = i->begin(); j != i->end(); ++j){
            all_abi.emplace_back(j->literal_no);
            // Loop over rest of elements
            for(auto k = std::next(j); k != i->end(); ++k){
                //constraint (3)
                mi_clauses_3.emplace_back(vector<int>{-j->literal_no, -k->literal_no, 0});
            }
            //constraint (5)
            //bidirectional yi = aij = xj | yj = bij = -xj
            // x = -p, cnf = (-x | -p) & (p | x)
            // x = p, cnf = (-x | p) & (-p | x)
            int x, y, ab;
            bool is_constant = false;
            if(j->c1_pi == "0" || j->c1_pi == "1"){
                    x = stoi(j->c1_pi);
                    is_constant = true;
            }
            else if(all_literals.find(j->c1_pi) == all_literals.end()){
                cout << "j->c1_pi: " << j->c1_pi << endl;
                cout << "error: mi constraint generator: literal no can't be found for c1(x)" << endl;
                exit(0);
            }
            else{
                x = all_literals.find(j->c1_pi)->second;
            }
            if(all_literals.find(j->c2_pi) == all_literals.end()){
                cout << "j->c2_pi: " << j->c2_pi << endl;
                cout << "error: mi constraint generator: literal no can't be found for c2(y)" << endl;
                exit(0);
            }
            else{
                y = all_literals.find(j->c2_pi)->second;
            }
            ab = j->literal_no;

            //encode
            // x = -p, cnf = (-x | -p) & (p | x)
            // x = p, cnf = (-x | p) & (-p | x)
            if(is_constant){
                vector<int> clause1;
                //yi = ab, (-yi || ab) & (yi || -ab)
                mi_clauses_5.emplace_back(vector<int>{-y, ab, 0});
                mi_clauses_5.emplace_back(vector<int>{y, -ab, 0});

                //ab = 1 | 0
                if(j->c1_pi == "0"){
                    mi_clauses_5.emplace_back(vector<int>{-ab, 0});
                }
                else if(j->c1_pi == "1"){
                    mi_clauses_5.emplace_back(vector<int>{ab, 0});
                }
                else{
                    cout << "error: during constant mapping, wrong name of c1_pi" << endl;
                    cout << "mi name: " << j->mi_name;
                    exit(0);
                }
            }
            else if(j->ab == 'a'){ //if aij
                //yi = aij, (-ab | y) & (ab | -y)
                mi_clauses_5.emplace_back(vector<int>{-ab, y, 0});
                mi_clauses_5.emplace_back(vector<int>{ab, -y, 0});

                //xi = aij, (-ab | x) & (ab | -x)
                mi_clauses_5.emplace_back(vector<int>{-ab, x, 0});               
                mi_clauses_5.emplace_back(vector<int>{ab, -x, 0});
            }
            else if(j->ab == 'b'){ //if bij
                //yi = bij, (-ab | y) & (ab | -y)
                mi_clauses_5.emplace_back(vector<int>{-ab, y, 0});
                mi_clauses_5.emplace_back(vector<int>{ab, -y, 0});

                //-xi = bij, (-ab | -x) & (ab | x)
                mi_clauses_5.emplace_back(vector<int>{-ab, -x, 0});
                mi_clauses_5.emplace_back(vector<int>{ab, x, 0});
            }
            else{
                cout << "error: wrong mi char" << endl;
                exit(0);
            }
        }
        all_abi.emplace_back(0);
        mi_clauses_3.emplace_back(all_abi);
    }
}

//bound f and g as MI: 
void mi_info::mi_mapper(vector<vector<mi>>& input_mapping){
    cout << "assigning literals to mi mapping" << endl << endl;
    string folderName = "overall_infos";
    int folderCreated = mkdir(folderName.c_str(), 0777);
    string path = folderName + "/mi_literals";
    ofstream outputfile(path);

    outputfile << "this file contains the literals no of the MI mapping" << endl;
    for(auto &i : input_mapping){
        for(auto &cur_mi : i){ //j is now a_ij or b_ij in terms of mi
            //give a_ij or b_ij literal no
            outputfile << cur_mi.mi_name << " : " << literal_no << " ";
            cur_mi.literal_no = literal_no;
            this->mi_mapping.emplace(cur_mi.mi_name, literal_no);
            all_literals.emplace(cur_mi.mi_name, literal_no++);
        }
        outputfile << endl;
    }
    mi_literals = this->mi_mapping;
    outputfile.close();
}

void mi_info::print_mi_clauses(){
    string path = "overall_infos/mi_clauses_constraint_3";
    ofstream outputfile(path);
    outputfile << "this file contains the clauses for constraint 3" << endl;
    for(auto i: this->mi_clauses_3){
        for(auto j:i){
            outputfile << j << " " ;
        }
        outputfile << endl;
    }
    outputfile.close();

    path = "overall_infos/mi_clauses_constraint_5";
    ofstream outputfile1(path);
    outputfile1 << "this file contains the clauses for constraint 5" << endl;
    for(auto i: this->mi_clauses_5){
        for(auto j:i){
            outputfile1 << j << " " ;
        }
        outputfile1 << endl;
    }
    outputfile1.close();
}

void mo_info::mo_mapper(vector<vector<mo>>& output_mapping){
    cout << "assigning literals to mo mapping" << endl << endl;
    string folderName = "overall_infos";
    int folderCreated = mkdir(folderName.c_str(), 0777);
    string path = folderName + "/mo_literals";
    ofstream mo_literals_file(path);

    mo_literals_file << "this file contains the literals no of the MO mapping" << endl;
    for(auto &i : output_mapping){
        int no = 0;
        for(auto &cur_mo : i){ //j is now a_ij or b_ij in terms of mi
            //give a_ij or b_ij literal no
            mo_literals_file << no++ << " " << cur_mo.mo_name << " : " << literal_no << " " << cur_mo.c1_po << " " << cur_mo.c2_po << " ";
            cur_mo.literal_no = literal_no;
            this->mo_mapping.emplace(cur_mo.mo_name, literal_no);
            all_literals.emplace(cur_mo.mo_name, literal_no++);
        }
        mo_literals_file << endl;
    }
    mo_literals = this->mo_mapping;
    mo_literals_file.close();    
}

void mo_info::mo_constraints_generator(const vector<vector<mo>>& output_mapping){

    //constraint (1): at least a pair of outputs should be matched
    cout << "generating constraint 1 of NP3 Boolean Matching: at least a pair of outputs should be matched" << endl << endl;
    vector<int> clause_constraint_1;
    cout << "generating constraint 4 of NP3 Boolean Matching: MO Mapping" << endl << endl ;

    cout << "generating constraint of picked mo" << endl << endl ;
    string path = "overall_infos/m0_clauses_constraint_4";
    ofstream outputfile2(path);
    outputfile2 << "this file contains the clauses for constraint 4" << endl;
    
    for(auto& i: output_mapping){ //row
        vector<int> picked_mo_temp;
        for(auto& j: i){ //collumn
            picked_mo_temp.emplace_back(j.literal_no);
            picked_mo_temp.emplace_back(0);
            //constraint (1)
            clause_constraint_1.emplace_back(j.literal_no);
            
            //constraint (4)
            int f, g, cd; //literal no's of gi, fj and cd_ij
            cd = j.literal_no;
            
            //finding literal no for c1_po 
            if(all_literals.find(j.c1_po) == all_literals.end()){
                cout << "mapping id: " << j.mo_name << endl;
                cout << "c1_po: " << j.c1_po << endl;
                cout << "error: mo constraint generator: literal no can't be found for c1(f)" << endl;
                exit(0);
            }
            else{
                f = all_literals.find(j.c1_po)->second;
            }

            //finding literal no for c2_po
            if(all_literals.find(j.c2_po) == all_literals.end()){
                cout << "mapping id: " << j.mo_name << endl;
                cout << "c2_po: " << j.c2_po << endl;
                cout << "error: mo constraint generator: literal no can't be found for c2(g)" << endl;
                exit(0);
            }
            else{
                g = all_literals.find(j.c2_po)->second;
            }

            //encode
            // x = -p, cnf = (-x | -p) & (p | x)
            // x = p, cnf = (-x | p) & (-p | x)
            
            if(j.cd == 'c'){ //if cij
                //gi == cij, (-cij | gi) & (cij | -gi)

                outputfile2 << j.mo_name << " " << j.c1_po << " " << j.c2_po << endl;
                outputfile2 << -cd << " " << g << endl;
                outputfile2 << cd << " " << -g << endl;
                outputfile2 << j.mo_name << " " << j.c1_po << " " << j.c2_po << endl;
                outputfile2 << -cd << " " << f << endl;
                outputfile2 << cd << " " << -f << endl;

                mo_clauses_4.emplace_back(vector<int>{-cd, g, 0});
                mo_clauses_4.emplace_back(vector<int>{cd, -g, 0});

                //fi == cij, (-cij | fi) & (cij | -fi)
                mo_clauses_4.emplace_back(vector<int>{-cd, f, 0});
                mo_clauses_4.emplace_back(vector<int>{cd, -f, 0});
            }
            else if(j.cd == 'd'){ //if dij
                //gi == dij, (-dij | gi) & ( dij | -gi)
                outputfile2 << j.mo_name << " " << j.c1_po << " " << j.c2_po << endl;
                outputfile2 << -cd << " " << g << endl;
                outputfile2 << cd << " " << -g << endl;
                outputfile2 << j.mo_name << " " << j.c1_po << " " << j.c2_po << endl;
                outputfile2 << -cd << " " << -f << endl;
                outputfile2 << cd << " " << f << endl;

                mo_clauses_4.emplace_back(vector<int>{-cd, g, 0});
                mo_clauses_4.emplace_back(vector<int>{cd, -g, 0});

                //-fi == dij, , (-dij | -gi) & (dij | gi)
                mo_clauses_4.emplace_back(vector<int>{-cd, -f, 0});
                mo_clauses_4.emplace_back(vector<int>{cd, f, 0});
            }
            else{
                cout << "error: wrong m0 char" << endl;
                exit(0);
            }
        } 
        picked_mo.emplace_back(picked_mo_temp);
    }
    outputfile2.close();
    
    clause_constraint_1.emplace_back(0);
    mo_clauses_1.emplace_back(clause_constraint_1);
    
    //constraint (2): an output of g can be mapped to at most one output of f 
    for(auto i = output_mapping.begin(); i != output_mapping.end(); ++i){
        for(auto j = i->begin(); j != i->end(); ++j){
            // Loop over rest of elements
            for(auto k = std::next(j); k != i->end(); ++k){
                mo_clauses_2.emplace_back(vector<int>{-j->literal_no, -k->literal_no, 0});
            }
        }
    }  
}

void mi_info::mi_printer(const vector<vector<mi>>& input_mapping){
    string path = "overall_infos/mi_mapping";
    ofstream outputfile(path);
    outputfile << "this file contains the mi mapping" << endl;
    for(auto i = input_mapping.begin(); i != input_mapping.end(); ++i){
        for(auto j = i->begin(); j != i->end(); ++j){
        outputfile << j->mi_name << " " << j->c1_pi << " " << j->c2_pi << "; ";
        }
    outputfile << endl;
    }
}

void mo_info::print_mo_clauses(){
    string path = "overall_infos/m0_clauses_constraint_1";
    ofstream outputfile(path);
    outputfile << "this file contains the clauses for constraint 1" << endl;
    for(auto i: this->mo_clauses_1){
        for(auto j:i){
            outputfile << j << " " ;
        }
        outputfile << endl;
    }
    outputfile.close();

    path = "overall_infos/m0_clauses_constraint_2";
    ofstream outputfile1(path);
    outputfile1 << "this file contains the clauses for constraint 2" << endl;
    for(auto i: this->mo_clauses_2){
        for(auto j:i){
            outputfile1 << j << " " ;
        }
        outputfile1 << endl;
    }
    outputfile1.close();
    
}

void mo_info::mo_printer(const vector<vector<mo>>& output_mapping){
    string path = "overall_infos/mo_mapping";
    ofstream outputfile(path);
    outputfile << "this file contains the mo mapping" << endl;
    for(auto i = output_mapping.begin(); i != output_mapping.end(); ++i){
        int no = 0;
        for(auto j = i->begin(); j != i->end(); ++j){
        
        outputfile << no++ << " " << j->literal_no << " " << j->mo_name << " " << j->c1_po << " " << " " << j->c2_po << "; " ;
        // outputfile << no++ << " " << j->literal_no << " " << j->mo_name << " " << j->c1_po << " " << c1.po_support_size.find(j->c1_po)->second << " " << j->c2_po  << " " << c2.po_support_size.find(j->c2_po)->second << "; ";
        }
    outputfile << endl;
    }
    outputfile.close();

    path = "overall_infos/picked_mo_clauses";
    ofstream outputfile1(path);
    outputfile1 << "this file contains the picked mo clauses" << endl;
    for(auto &i: picked_mo){
        for(auto &j: i){
            outputfile1 << j << " ";
        }
        outputfile1 << endl;
    }
    outputfile1.close();
}
