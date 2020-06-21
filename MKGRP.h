#ifndef MKGRP_H
#define MKGRP_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <iostream>
#include <QList>
#include <math.h>
#include "Mount.h"
#include "structures.h"

using namespace std;
extern QList<Mount> *montajes;
extern Sesion daLoguer;
extern bool login;

class MKGRP
{
private:
    string name;
public:
    MKGRP(string);
    void Ejecutar();
    int buscarGrupo(string);
    int Get_Id_Group();
    void agregarUsers(string);
    void guardarJournal(char* , int , int, char*);
    int buscarBit(FILE*, char,char);
};

#endif // MKGRP_H
