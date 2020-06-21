#ifndef MKDIR_H
#define MKDIR_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <libgen.h>
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <math.h>
#include "Mount.h"
#include "structures.h"

using namespace std;
extern QList<Mount> *montajes;
extern Sesion daLoguer;
extern bool login;

class MKDir
{
private:
    string path;
    bool padre;
public:
    MKDir(string, bool);
    void Ejecutar();
    int crearCarpeta(string, bool);
    void guardarJournal(char* , int , int, char*);
    int buscarCarpetaArchivo(FILE*, char*);
    int byteInodoBloque(FILE*, int, char);
    int nuevaCarpeta(FILE *, char, bool, char*, int);
    int buscarContentLibre(FILE*, int, InodoTable &, BloqueCarpeta &, BloqueApuntadores &, int &, int &,int &);
    int buscarBit(FILE*, char,char);
    bool permisosDeEscritura(int, bool, bool);
    InodoTable crearInodo(int, char, int);
    BloqueCarpeta crearBloqueCarpeta();
};

#endif // MKDIR_H
