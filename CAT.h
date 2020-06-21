#ifndef CAT_H
#define CAT_H
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

class CAT
{
private:
    string path;
public:
    CAT(string);
    void Ejecutar();
    int buscarCarpetaArchivo(FILE*, char*);
    int byteInodoBloque(FILE*, int, char);
    bool permisosDeLectura(int, bool, bool);
};

#endif // CAT_H
