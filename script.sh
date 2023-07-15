#!/bin/bash

alias iccad="gcc -I/home/des/NewBF/abc/src/code -I/home/des/NewBF/abc/src -Wall -g -c -o bmatch.o bmatch.cpp  && g++ -g -o bmatch bmatch.o libabc.a -lm -ldl -lreadline -lpthread && ./bmatch input match"

alias remove_bmatch="rm circuit_1_cnf_.cnf && rm circuit_1_aig.aig && rm bmatch && rm bmatch.o && rm circuit_1_new.v && rm circuit_2_new.v && rm output.txt &&"

alias rm_tc="rm input && rm circuit_1.v && rm circuit_2.v"

alias rm_dr="rm -r c1_data && rm -r c2_data && rm -r c1_po && rm -r c2_po && rm -r c1_each_po && rm -r c2_each_po"

alias rm_all="rm -r c1_data && rm -r c2_data && rm -r c1_po && rm -r c2_po && rm -r c1_each_po && rm -r c2_each_po && rm input && rm circuit_1.v && rm circuit_2.v"