#ifndef CHMOD_H
#define CHMOD_H
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

class CHMOD
{
private:
    string path;
    int ugo;
    bool recursivo;
public:
    CHMOD(string, int, bool);
    void Ejecutar();
    int buscarCarpetaArchivo(FILE *, char*);
    void cambiarPermisosRecursivo(FILE* , int , int );
    int byteInodoBloque(FILE *,int , char );
    void guardarJournal(char*, int, int, char*);
};

#endif // CHMOD_H
