#include "mo_generator.h"

vector<vector<mo>> build_mo(){
    vector<vector<mo>> mo_mapping;
    for(int i = 0; i < c2.no_of_pos; i++){ //iterate cir1 pis, x
        vector<mo> cur;
        for(int j = 0; j < c1.no_of_pos; j++){ //iterate cir2 pis, y
            for(int k = 0; k <= 1; k++){
                string mo_name = (k == 0 ? "c" : "d") + to_string(i+1) + to_string(j+1);
                cur.emplace_back(mo(mo_name, c1.pos[j].po_name, c2.pos[i].po_name, (k == 0 ? 'c': 'd'),  i+1 , j+1));
            }
        }
        mo_mapping.emplace_back(cur);
    }
    return mo_mapping;
}

void mo_printer(vector<vector<mo>>& mo_mapping){
    cout << "*********************************************" << endl;
    cout << " printing output mapping matrix " << endl << endl;
    for(const auto& row : mo_mapping) {
        int i = 0;
        for(const auto& elem : row) {
            cout << i++ << " " << elem << " "; // << operator is now overloaded for mo class
        }
        cout << "\n";
    }
    cout << "*********************************************" << endl << endl;
}