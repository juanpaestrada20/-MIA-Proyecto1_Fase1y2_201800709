#ifndef MKFILE_H
#define MKFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <iostream>
#include <QFileInfo>
#include <QList>
#include <math.h>
#include <libgen.h>
#include "Mount.h"
#include "structures.h"

using namespace std;
extern QList<Mount> *montajes;
extern Sesion daLoguer;
extern bool login;


class MKFILE
{
private:
    string path;
    bool p;
    int size;
    string cont;
public:
    MKFILE(string, bool, int, string);
    void Ejecutar();
    int crearArchivo(string, bool, int, string);
    void guardarJournal(char*, int, int, char*);
    int nuevoArchivo(FILE*, char, bool, char*, int, string, int,char*);
    int buscarCarpetaArchivo(FILE*, char*);
    int byteInodoBloque(FILE*, int, char);
    int buscarContentLibre(FILE*, int, InodoTable &, BloqueCarpeta &, BloqueApuntadores &, int &, int &,int &);
    bool permisosDeEscritura(int, bool, bool);
    int buscarBit(FILE*, char,char);
    int nuevaCarpeta(FILE *, char, bool, char *, int);
    InodoTable crearInodo(int, char, int);
    BloqueCarpeta crearBloqueCarpeta();
};

#endif // MKFILE_H
