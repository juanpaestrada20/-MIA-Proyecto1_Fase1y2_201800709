#!/bin/bash


echo "Compilacion de Bison..."
bison -o sintactico.cpp --defines=sintactico.h sintactico.y

echo "Compilacion de Flex..."
flex -o lexico.cpp --header-file=lexico.h lexico.l
