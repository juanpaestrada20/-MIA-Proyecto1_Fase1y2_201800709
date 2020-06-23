#ifndef RMGRP_H
#define RMGRP_H
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

class RMGRP
{
private:
    string name;
public:
    RMGRP(string);
    void Ejecutar();
    int buscarGrupo(string);
    void EliminarGrupo(string);
    void guardarJournal(char*, int, int, char*);
};

#endif // RMGRP_H
