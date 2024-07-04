#include "projection.h"

void configure_projection(){
    //add allowproj to all literals
    all_literals.emplace("allowProj", literal_no++);
    int allowProj_no;
    int no = c2.no_of_pos;
    int mo = c1.no_of_pos;
    cout << "configuring_projection: " << endl << endl;
    string folderName = "overall_infos";
    //int folderCreated = mkdir(folderName.c_str(), 0777);
    string path = folderName + "/projection_clauses";
    ofstream outputfile(path);

    for(int i = 1; i <= no; i++){
        for(int j = i + 1; j <= no; j++){
            for(int k = 1; k <= mo; k++){
                int c_ik, c_jk, d_ik, d_jk;
                string cjk, cik, dik, djk;
                cjk = "c" + to_string(j) + to_string(k);
                cik = "c" + to_string(i) + to_string(k);
                djk = "d" + to_string(j) + to_string(k);
                dik = "d" + to_string(i) + to_string(k);
                outputfile << "cik " << cik << "; " << "cjk " << cjk << "; " << "dik " << dik << "; " << "djk " << djk << ";" << endl;

                //cjk
                if(output_mapping_info.mo_literals.find(cjk) == output_mapping_info.mo_literals.end()){
                    cout << "cant find " << cjk << " during " << j <<" "<< k << endl;   
                    exit(0);                                                                                                                                                                                                                                                                                                                                                                                         
                } 
                else{
                    c_jk = output_mapping_info.mo_literals.find(cjk)->second;
                }
                //cik
                if(output_mapping_info.mo_literals.find(cik) == output_mapping_info.mo_literals.end()){
                    cout << "cant find " << cik << " during " << i <<" "<< k << endl;   
                    exit(0);                                                                                                                                                                                                                                                                                                                                                                                         
                } 
                else{
                    c_ik = output_mapping_info.mo_literals.find(cik)->second;
                }
                //djk
                if(output_mapping_info.mo_literals.find(djk) == output_mapping_info.mo_literals.end()){
                    cout << "cant find " << djk << " during " << j <<" "<< k << endl;   
                    exit(0);                                                                                                                                                                                                                                                                                                                                                                                         
                } 
                else{
                    d_jk = output_mapping_info.mo_literals.find(djk)->second;
                }
                //cik
                if(output_mapping_info.mo_literals.find(dik) == output_mapping_info.mo_literals.end()){
                    cout << "cant find " << dik << " during " << i <<" "<< k << endl;   
                    exit(0);                                                                                                                                                                                                                                                                                                                                                                                         
                } 
                else{
                    d_ik = output_mapping_info.mo_literals.find(dik)->second;
                }

                //allowProj
                if(all_literals.find("allowProj") == all_literals.end()){
                    cout << "cant find " << "allowProj" << " during " << i <<" "<< k << endl;   
                    exit(0);                                                                                                                                                                                                                                                                                                                                                                                         
                } 
                else{
                    allowProj_no = all_literals.find("allowProj")->second;
                }

                //(allowProj | -cik | -cjk)
                output_mapping_info.projection_clauses.emplace_back(vector<int>{allowProj_no, -c_ik, -c_jk, 0});
                //(allowProj | -cik | -djk)
                output_mapping_info.projection_clauses.emplace_back(vector<int>{allowProj_no, -c_ik, -d_jk, 0});
                //(allowProj | -dik | -cjk)
                output_mapping_info.projection_clauses.emplace_back(vector<int>{allowProj_no, -d_ik, -c_jk, 0});
                //(allowProj | -dik | -djk)
                output_mapping_info.projection_clauses.emplace_back(vector<int>{allowProj_no, -d_ik, -d_jk, 0});
            }
        }
    }
    output_mapping_info.allow_proj_off.emplace_back(-allowProj_no);
    output_mapping_info.allow_proj_off.emplace_back(0);

    output_mapping_info.allow_proj_on.emplace_back(allowProj_no);
    output_mapping_info.allow_proj_on.emplace_back(0);
    outputfile.close();
}

void print_projection_clauses(){
    cout << "printing clauses of projection" << endl << endl;
    string folderName = "overall_infos";
    string path = folderName + "/projection_clauses";
    ofstream outputfile(path, ios::app);
    outputfile << endl << "projection_clauses" << endl;
    for(auto const & i: output_mapping_info.projection_clauses){
        for(auto const &j: i){
            outputfile << j << " ";
        }
        outputfile << endl;
    }
    outputfile << endl;

    outputfile << "allow proj off" << endl;
    for(auto const &i : output_mapping_info.allow_proj_off){
        outputfile << i << " ";
    }
    outputfile << endl << endl;

    outputfile << "allow proj on" << endl;
    for(auto const &i : output_mapping_info.allow_proj_on){
        outputfile << i << " ";
    }
    outputfile << endl;
    outputfile.close();
    

}