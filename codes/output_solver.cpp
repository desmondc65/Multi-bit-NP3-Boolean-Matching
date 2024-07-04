#include "output_solver.h"

// void output_solver(){
//     while(!optimal && !timeout){
//         if(new pair can be found by output SAT){
//             add a new output matching pair to R;
//         }
//         else{
//             if(projection is disabled){
//                 enable projection;
//             }
//             else{
//                 forbid the curring matching result R;
//                 remove the last added pair from R;
//                 disable projection;
//             }
//             continue;
//         }

//         input_solver();
//         if(R cannot be satisfied){
//             forbid the current matching result R;
//             remove the last added pair from R;
//         }
//     }
// }

void output_solver(){
    vector<pair<string, int>> mo_literals_sorted;
    //initialize mo_literals_sorted vector;
    for(auto &i: output_mapping_info.mo_literals){
        mo_literals_sorted.emplace_back(i);
    }
    sort(mo_literals_sorted.begin(), mo_literals_sorted.end(), [](const pair<string, int>& a, const pair<string, int>& b){
        return a.second < b.second;
    });
    for(auto &i: mo_literals_sorted){
        cout << i.second << " " ;
    }
    
    //bool optimal = true;
    while((get_time() < 3500)){
        string new_output_pair = output_sat();
        if(new_output_pair != "NO Pair left"){ //if a new pair can be found

        }
    }
}

string output_sat(){ // find a valid NP3 output pair
    string folderName1 = "overall_infos";
    string path1 = folderName1 + "/output_test_log";
    CaDiCaL::Solver * solver = new CaDiCaL::Solver;

    //add clauses of constraint 1
    for(auto const &i: output_mapping_info.mo_clauses_1){
        for(auto const &j: i){
            solver->add(j);
        }
    }

    //add clauses of constraint 2
    for(auto const &i: output_mapping_info.mo_clauses_2){
        for(auto const &j: i){
            solver->add(j);
        }
    }

    //add clauses of projection
    for(auto const &i: output_mapping_info.projection_clauses){
        for(auto const &j: i){
            solver->add(j);
        }
    }

    //add projection on or off clause
    if(output_mapping_info.projection_on){
        for(auto const &i: output_mapping_info.allow_proj_on){
            solver->add(i);
        }
    }
    else{
        for(auto const &i: output_mapping_info.allow_proj_off){
            solver->add(i);
        }
    }

    //add clauses of matched output to avoid picking them again
    // for(auto &i: matched_output_clauses)
}

void sat_test_output(){
    string folderName1 = "overall_infos";
    string path1 = folderName1 + "/output_test_log";
    ofstream file(path1);
    file << "sat test output:" << endl;
    CaDiCaL::Solver * solver = new CaDiCaL::Solver;
    
    for(auto const &i: output_mapping_info.picked_mo[1]){
        solver->add(-i);
    }
    for(auto const &i: output_mapping_info.mo_clauses_1){
        for(auto const &j: i){
            solver->add(j);
        }
    }
    for(auto const &i: output_mapping_info.mo_clauses_2){
        for(auto const &j: i){
            solver->add(j);
        }
    }
    for(auto const &i: output_mapping_info.projection_clauses){
        for(auto const &j: i){
            solver->add(j);
        }
    }
    for(auto const &i: output_mapping_info.allow_proj_off){
        solver->add(i);
    }
    int res = solver->solve (); 
    if(res == 20){
        file << "UNSAT" << endl;
    }
    else if(res == 10){
        file << "sat" << endl;
        for(auto const &i : output_mapping_info.mo_literals){
            if(solver->val(i.second) > 0)
            file << i.first << " " << solver->val(i.second) << endl;
        }
    }
    else if(res == 0){
        file << "unsolved" << endl;
    }
    else{
        file << "error in sat test output" << endl;
    }
    file << endl;
       // Solve instance.

    res = solver->solve (); 
    if(res == 20){
        file << "UNSAT" << endl;
    }
    else if(res == 10){
        file << "sat" << endl;
        for(auto const &i: output_mapping_info.output_mapping){
            for(auto const &j: i){
                file << j.mo_name << " " << ((solver->val(j.literal_no) > 0) ? "1" : "0") << " ";
            }
            file  << endl;
        }
    }
    else if(res == 0){
        file << "unsolved" << endl;
    }
    else{
        file << "error in sat test output" << endl;
    }
    file << endl;
    file.close();
    
}

void sat_test_input(){
    CaDiCaL::Solver * solver = new CaDiCaL::Solver;
    for(auto const &i: output_mapping_info.mo_clauses_1){
        for(auto const &j: i){
            solver->add(j);
        }
    }
    for(auto const &i: output_mapping_info.mo_clauses_2){
        for(auto const &j: i){
            solver->add(j);
        }
    }
    int res = solver->solve (); 
    cout << res << endl;  
       // Solve instance.
    
}
