#ifndef RMUSR_H
#define RMUSR_H
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

class RMUSR
{
private:
    string user;
public:
    RMUSR(string);
    void Ejecutar();
    bool BuscarUsuario(string);
    void EliminarUsuario(string);
};

#endif // RMUSR_H
