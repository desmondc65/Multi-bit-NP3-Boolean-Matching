#!/bin/bash

alias iccad="rm_obj; make; ./bmatch input match"

alias des="./bmatch input match"

alias remove_bmatch="[ -e circuit_1_cnf_.cnf ] && rm circuit_1_cnf_.cnf; [ -e circuit_1_aig.aig ] && rm circuit_1_aig.aig; [ -e bmatch ] && rm bmatch; [ -e bmatch.o ] && rm bmatch.o; [ -e circuit_1_new.v ] && rm circuit_1_new.v; [ -e circuit_2_new.v ] && rm circuit_2_new.v; [ -e output.txt ] && rm output.txt"

alias rm_tc="[ -e input ] && rm input; [ -e circuit_1.v ] && rm circuit_1.v; [ -e circuit_2.v ] && rm circuit_2.v"

alias rm_dr="[ -e c1_data ] && rm -r c1_data; [ -e c2_data ] && rm -r c2_data; [ -e c1_po ] && rm -r c1_po; [ -e c2_po ] && rm -r c2_po; [ -e c1_each_po ] && rm -r c1_each_po; [ -e c2_each_po ] && rm -r c2_each_po"

alias rm_obj="[ -e obj ] && rm -r obj; [ -e bmatch ] && rm -r bmatch"
alias rm_all="\
[ -e c1_data ] && rm -r c1_data; \
[ -e c2_data ] && rm -r c2_data; \
[ -e c1_po ] && rm -r c1_po; \
[ -e c2_po ] && rm -r c2_po; \
[ -e c1_each_po ] && rm -r c1_each_po; \
[ -e c2_each_po ] && rm -r c2_each_po; \
[ -e input ] && rm input; \
[ -e circuit_1.v ] && rm circuit_1.v; \
[ -e bmatch.o ] && rm bmatch.o; \
[ -e bmatch ] && rm bmatch; \
[ -e obj ] && rm obj; \
[ -e circuit_2.v ] && rm circuit_2.v"