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

#ifndef utility_h
#define utility_h
#endif

using namespace std;

bool readFileEquivalent(string path);

#if defined(ABC_NAMESPACE)
namespace ABC_NAMESPACE
{
#elif defined(__cplusplus)
extern "C"
{
#endif

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
void   Abc_Start();
void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
typedef struct Abc_Frame_t_ Abc_Frame_t;
Abc_Frame_t * Abc_FrameGetGlobalFrame();
int Cmd_CommandExecute( Abc_Frame_t * pAbc, const char * sCommand );
#if defined(ABC_NAMESPACE)
}
using namespace ABC_NAMESPACE;
#elif defined(__cplusplus)
}
#endif

vector<vector<int>> generatePermutations(int n) {
    vector<vector<int>> permutations;
    vector<int> elements(n);
    for (int i = 0; i < n; ++i) {
        elements[i] = i + 1;
    }
    // Generate permutations
    do {
        permutations.push_back(elements);
    } while (next_permutation(elements.begin(), elements.end()));

    return permutations;
}

bool cecTest(string cir1, string cir2, Abc_Frame_t * pAbc){
    
    string resultPath = "output-cec.txt";
    cout.flush();
    FILE *saveStdout = stdout;
    bool finalResult = true;
    stdout = fopen(resultPath.c_str(), "w");
    if (stdout != NULL) {
        string command;
        command = "cec -s " + cir1 + " " + cir2;
        
              
        if (Cmd_CommandExecute(pAbc, command.c_str())){
            cout << "Cannot execute command \"" << command << "\".\n";
            exit(1);
        }
        
        fflush(stdout);
        
        finalResult = readFileEquivalent(resultPath) && finalResult;
        fclose(stdout);
        stdout = saveStdout;
    } else {
        cout << "Can't write file:" << resultPath << endl;
        exit(1);
    }
    if(std::remove(cir1.c_str())){
        cout << "Can't delete fie:" << cir1 << endl;
    }
    if(std::remove(cir2.c_str())){
        cout << "Can't delete fie:" << cir2 << endl;
    }
    if(std::remove(resultPath.c_str())){
        cout << "Can't delete fie:" << resultPath << endl;
    }

    return finalResult;
}

bool readFileEquivalent(string path){
    bool equivalent = false;
    ifstream readFile(path, ios::in);
    if(readFile.is_open()){
        string result;
        while (getline(readFile, result)){
            if(result.find("Networks are equivalent") != string::npos){
                equivalent = true;
            }
        }
        readFile.close();
    }else{
        cout << "Can't read writeFile:" << path << endl;
        exit(1);
    }
    ofstream writeFile(path, std::ios::trunc);
    if (writeFile.is_open()) {
        writeFile.close();
    } else {
        cout << "Can't clean writeFile:" << path << endl;
        exit(1);
    }
    return equivalent;
}

void printPermutations(const vector<vector<int>>& permutations) {
    for (const vector<int>& permutation : permutations) {
        for (int element : permutation) {
            cout << element << ' ';
        }
        cout << '\n';
    }
}

vector<vector<int>> enumerateBits(int n) {
    vector<vector<int>> sequences;

    // Calculate the total number of possible sequences
    int totalSequences = 1 << n;

    // Iterate through all possible sequences
    for (int i = 0; i < totalSequences; i++) {
        // Convert the decimal number to binary representation
        vector<int> bits;
        int num = i;
        for (int j = 0; j < n; j++) {
            bits.push_back(num % 2);
            num /= 2;
        }
        // Reverse the bits to represent the correct sequence
        reverse(bits.begin(), bits.end());

        // Add the sequence to the vector
        sequences.push_back(bits);
    }

    return sequences;
}
