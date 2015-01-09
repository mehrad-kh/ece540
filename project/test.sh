#! /bin/bash

#$1 name of the program to be executed

ssfe hw5/$1.c

./project hw5/$1.tmp hw5/$1.spx

ssbe hw5/$1

sim-safe -redir:prog mehrad hw5/$1.ss