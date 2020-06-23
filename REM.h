#ifndef REM_H
#define REM_H
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


class REM
{
private:
    string path;
    bool edit;
public:
    REM(string, bool);
    void Ejecutar();
    int buscarCarpetaArchivo(FILE*, char*);
    int byteInodoBloque(FILE*, int, char);
    bool permisosEscrituraRecursivo(FILE*, int);
    bool permisosDeEscritura(int, bool, bool);
    void bloqueCarpetaArchivo(FILE*, char*, int &, int &,int &,int &);
    void eliminarCarpetaArchivo(FILE*, int);
    void guardarJournal(char*, int, int, char*);
};

#endif // REM_H
