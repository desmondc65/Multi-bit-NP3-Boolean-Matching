#include "headers/bmatch_new.h"

using namespace std;

int main( int argc, char * argv[] )
{
    freopen("log.txt", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    auto start = chrono::high_resolution_clock::now();
    if ( argc != 3 )
    {
        printf( "Wrong number of command-line arguments.\n" );
        return 1;
    }

    //Initialization
    Abc_Frame_t * pAbc;
    string input_name = argv[1]; //read input 
    string output_name = argv[2]; //get output name

    //read input
    read_input(input_name, c1_path_v, c1_number_of_inputs, c2_path_v, c2_number_of_inputs, c1_bus_size_input, c2_bus_size_input, c1_bus_input_1, c2_bus_input_1, c1_bus_input_2, c2_bus_input_2);
    prepare_input(c1_path_v, c1_new_path_v);
    prepare_input(c2_path_v, c2_new_path_v);
    
    //start of np3 boolean matching
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();

    //read_strash_aig_cnf of circuits 1 & 2
    read_strash_aig_cnf(pAbc, c1_new_path_v);
    read_strash_aig_cnf(pAbc, c2_new_path_v);

    //split verilog files according to POs
    verilog_parser(c1_new_path_v + "_simplified.v", c1_verilog_assigns, c1_vec_pis, c1_vec_pos, "c1");
    verilog_parser(c2_new_path_v + "_simplified.v", c2_verilog_assigns, c2_vec_pis, c2_vec_pos, "c2");

    //write pis and pos to files
    write_pi_po_assigns(c1_vec_pis, c1_vec_pos, c1_verilog_assigns, "c1_data", "c1");
    write_pi_po_assigns(c2_vec_pis, c2_vec_pos, c2_verilog_assigns, "c2_data", "c2");

    //save lines to po
    sort_assigns_to_po1(c1_Pos, c1_vec_pos, c1_verilog_assigns, c1_new_path_v, c1_name);
    sort_assigns_to_po2(c2_Pos, c2_vec_pos, c2_verilog_assigns, c2_new_path_v, c2_name);

    //sort po according to support size in ascending order
    sort(c1_Pos.begin(), c1_Pos.end(), compareBySupportSize);
    sort(c2_Pos.begin(), c2_Pos.end(), compareBySupportSize);
    
    print_po_self(c1_Pos, "circ1");
    print_po_self(c2_Pos, "circ2");

    //set new wires for all pos
    setNewWiresForAll(c1_Pos);
    setNewWiresForAll(c2_Pos);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Preprocess Time: " << duration.count() << " milliseconds" << std::endl;
    
    string folderName1 =   "c1_each_po/";
    int folderCreated1 = mkdir(folderName1.c_str(), 0777);

    string folderName2 =   "c2_each_po/";
    int folderCreated2 = mkdir(folderName2.c_str(), 0777);

    output_solver(c1_Pos, c2_Pos, pAbc);
    //print_output(matches);

    //End of program
    Abc_Stop();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
    fclose(stdout);
    return 0;
}
