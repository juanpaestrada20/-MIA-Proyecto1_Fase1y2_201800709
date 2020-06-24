#ifndef FIND_H
#define FIND_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <iostream>
#include <QList>
#include <QFileInfo>
#include <math.h>
#include "Mount.h"
#include "structures.h"

using namespace std;
extern QList<Mount> *montajes;
extern Sesion daLoguer;
extern bool login;
extern QList<Files> *files;


class FIND
{
public:
    string path;
    string name;
    string tab;
    string ruta;
    FIND(string, string);
    void Ejecutar();
    void guardarJournal(char*, int, int, char*);
    int buscarCarpetaArchivo(FILE*, char*);
    int byteInodoBloque(FILE*, int, char);
    Files buscarRuta(string);
    void getRuta(FILE*, string &, int, int);
    void imprimirRuta(string);
};

#endif // FIND_H
