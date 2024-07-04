#include <iostream>
#include "mi.h"

void mi::configure(string c1_pi, string c2_pi, char ab, int i, int j, string mi_name){
    this->c1_pi = c1_pi;
    this->c2_pi = c2_pi;
    this->ab = ab;
    this->i = i;
    this->j = j;
    this->mi_name = mi_name;
}