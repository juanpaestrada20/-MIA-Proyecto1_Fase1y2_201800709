#ifndef EDIT_H
#define EDIT_H
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
#include "REM.h"
#include "MKFILE.h"

using namespace std;
extern QList<Mount> *montajes;
extern QList<MKFILE> *archivos;
extern Sesion daLoguer;
extern bool login;

class EDIT
{
private:
    string path;
    string cont;
public:
    EDIT(string, string);
    void Ejecutar();
    int buscarCarpetaArchivo(FILE*, char*);
    int byteInodoBloque(FILE*, int, char);
    bool permisosDeLectura(int, bool, bool);
    bool permisosDeEscritura(int, bool, bool);
    MKFILE buscarArchivo(string);
    void guardarJournal(char*, int, int, char*);
};

#endif // EDIT_H
