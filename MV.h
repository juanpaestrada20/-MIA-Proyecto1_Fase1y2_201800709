#ifndef MV_H
#define MV_H
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

class MV
{
private:
    string path;
    string dest;
public:
    MV(string, string);
    void Ejecutar();
    int buscarCarpetaArchivo(FILE*, char*);
    int byteInodoBloque(FILE*, int, char);
    bool permisosLecturaRecursivo(FILE*, int);
    bool permisosDeLectura(int, bool, bool);
    bool permisosDeEscritura(int, bool, bool);
    void moverCarpetaArchivo(FILE*, int, char*, int);
    int buscarBit(FILE*, char,char);
    void bloqueCarpetaArchivo(FILE*, char*, int &, int &,int &,int &);
    void guardarJournal(char*, int, int, char*);
};

#endif // MV_H
