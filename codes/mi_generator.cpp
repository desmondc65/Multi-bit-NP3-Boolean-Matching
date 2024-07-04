#include "mi_generator.h"

vector<vector<mi>> build_mi(vector<string>& c1_pi, vector<string>& c2_pi){
    vector<vector<mi>> mi_mapping;
    for(int i = 0; i < c2_pi.size(); i++){ //iterate cir1 pis, x
        vector<mi> cur;
        mi temp1;
        mi temp2;
        for(int j = 0; j < c1_pi.size(); j++){ //iterate cir2 pis, y
            for(int k = 0; k <= 1; k++){
                mi temp;
                string mi_name = (k == 0? "a" : "b") + to_string(i+1) + to_string(j+1);
                temp.configure(c1_pi[j], c2_pi[i], (k == 0 ? 'a': 'b'), i+1 , j+1, mi_name);
                cur.emplace_back(temp);
            }
        }
        temp1.configure("0", c2_pi[i], 'a', i+1, c1_pi.size()+1, "a" + to_string(i+1) + to_string(c1_pi.size()+1));
        cur.emplace_back(temp1);
        temp2.configure("1", c2_pi[i], 'b', i+1, c1_pi.size()+1, "b" + to_string(i+1) + to_string(c1_pi.size()+1));
        cur.emplace_back(temp2);
        mi_mapping.emplace_back(cur);
    }
    return mi_mapping;
}

void mi_printer(vector<vector<mi>>& mi_mapping){
    cout << "*********************************************" << endl;
    cout << " printing input mapping matrix " << endl << endl;
    for(const auto& row : mi_mapping) {
        int i = 1;
        for(const auto& elem : row) {
            cout << i++ << " " << "mi: " << elem.mi_name << " " << elem.c1_pi << " " << elem.c2_pi << " "; // << operator is now overloaded for mi class
        }
        cout << "\n";
    }
    cout << "*********************************************" << endl << endl;
}