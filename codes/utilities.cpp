#include "utilities.h"

void print_all_literals(){
    string path = "overall_infos/all_literals";
    ofstream all(path);
    all <<"this file constains all literals and their encoded literal no:" << endl;
    for(const auto &i : all_literals){
        all << i.first << " " << i.second << endl;
    }
    all.close();
}

void get_po_support_size(){
    cout << "getting po support size" << endl << endl;
    ifstream info("info.txt");
    string line;
    //traverse to Structural support info:
    while(getline(info, line)){
        if(line == "Structural support info:"){
            break;
        }
    }
    
    int c1_po_sz = c1.no_of_pos;
    for(int i = 0; i < c1_po_sz; i++){
        getline(info, line); //get po info
        string no, po_name, op, cone, equal, no1, supp, eq2, ss;
        istringstream iss(line);
        iss >> no >> po_name >> op >> cone >> equal >> no1 >> supp >> eq2 >> ss;
        ss.pop_back();
        // cout << ss << endl;
        int supsize = stoi(ss);
        c1.pos.emplace_back(po(po_name, supsize));
        c1.po_support_size.emplace(po_name, supsize);
    }
    
    // for(auto i: c1.pos){
    //    cout << i.po_name << " " << i.support_size << endl;
    // }

    //traverse to Structural support info:
    while(getline(info, line)){
        if(line == "Structural support info:"){
            break;
        }
    }

    int c2_po_sz = c2.no_of_pos;
    for(int i = 0; i < c2_po_sz; i++){
        getline(info, line); //get po info

        string no, po_name, op, cone, equal, no1, supp, eq2, ss;
        istringstream iss(line);
        iss >> no >> po_name >> op >> cone >> equal >> no1 >> supp >> eq2 >> ss;
        ss.pop_back();
        // cout << ss << endl;
        int supsize = stoi(ss);
        c2.pos.emplace_back(po(po_name, supsize));
        c2.po_support_size.emplace(po_name, supsize);
    }
    // for(auto i: c2.po_support_size){
    //     cout << i.first << " " << i.second << endl;
    // }

    // for(auto i: c2.pos){
    //    cout << i.po_name << " " << i.support_size << endl;
    // }
    info.close();
}

void print_time(const string& message) {
    cout << message << ": " << fixed << setprecision(3) << get_time() << " seconds" << endl << endl;
}


double get_time() {
    auto current_time = chrono::steady_clock::now();
    auto elapsed_time = current_time - program_start_time;
    return chrono::duration<double, ratio<1, 1>>(elapsed_time).count();
}