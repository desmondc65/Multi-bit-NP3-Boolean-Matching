#ifndef MI_H
#define MI_H

#include <string>

using namespace std;

class mi{
public:
    //varibles
    string mi_name;
    string c1_pi;
    string c2_pi;
    char ab;
    int i;
    int j;
    int literal_no;

    //functions
    void configure(string c1_pi, string c2_pi, char ab, int i, int j, string mi_name);
};

#endif
