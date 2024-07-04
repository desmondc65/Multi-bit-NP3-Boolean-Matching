#include "bmatch.h"

using namespace std;

int main( int argc, char * argv[] )
{
    freopen("log.txt", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ofstream file("warnings.txt");
    auto start = chrono::high_resolution_clock::now();

    if ( argc != 3 )
    {
        printf( "Wrong number of command-line arguments.\n" );
        return 1;
    }

    //Initialization
    Abc_Frame_t * pAbc;
    string input_name = argv[1]; //read input 
    string output_name = argv[2]; //get output name---------------------------------------------
    literal_no = 1;

    //read input
    read_input(input_name, c1, c2);
    prepare_input(c1);
    prepare_input(c2);
    
    //start of np3 boolean matching
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    fclose(stdout);

    freopen("info.txt", "w", stdout);
    //read_strash_aig_cnf of circuits 1 & 2
    read_strash_aig_cnf(pAbc, c1);
    read_strash_aig_cnf(pAbc, c2);
    fclose(stdout);

    freopen("log.txt", "w", stdout);

    //split verilog files according to POs
    verilog_parser(c1);
    verilog_parser(c2);

    //write pis and pos to files
    write_pi_po_assigns(c1);
    write_pi_po_assigns(c2);
    
    auto preprocess_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(preprocess_time - start);
    cout << "Preprocess Time: " << duration.count() << " milliseconds" << endl <<endl;

    //two-step:

    //preprocess
    get_po_support_size();
    c1.sort_circuit_po();
    c2.sort_circuit_po();

    //generate MI
    input_mapping_info.input_mapping = build_mi(c1.pis_name, c2.pis_name);
    //mi_printer(input_mapping);

    //generate MO
    output_mapping_info.output_mapping = build_mo();
    // mo_printer(output_mapping);

    //generate CNF of circuit 1
    c1.verilog_to_cnf();
    c1.print_clauses();
    c1.write_literals_to_file();

    //generate CNF of circuit 2
    c2.verilog_to_cnf();
    c2.print_clauses();
    c2.write_literals_to_file();

    //NP3 Boolean Matching constraints
    //input section:
    input_mapping_info.mi_mapper(input_mapping_info.input_mapping); 
    input_mapping_info.mi_printer(input_mapping_info.input_mapping);
    input_mapping_info.mi_constraints_generator(input_mapping_info.input_mapping); 
    input_mapping_info.print_mi_clauses();

    //output section:
    output_mapping_info.mo_mapper(output_mapping_info.output_mapping);
    output_mapping_info.mo_constraints_generator(output_mapping_info.output_mapping);
    output_mapping_info.print_mo_clauses();
    output_mapping_info.mo_printer(output_mapping_info.output_mapping);
    //end of NP3 Boolean Matching constraints

    //configure allow projection
    configure_projection();
    print_projection_clauses();

    //output solver
    // output_solver();
    sat_test_output();

    //End of program

    //write all literals to file 
    print_all_literals();

    Abc_Stop();

    auto two_step = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(two_step - preprocess_time);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl << endl;
    duration = chrono::duration_cast<chrono::milliseconds>(two_step - start);
    cout << "total time: " << duration.count() << " milliseconds" << endl << endl;
    
    fclose(stdout);
    return 0;
}

///  cd home/des/files/iccad2023/code 