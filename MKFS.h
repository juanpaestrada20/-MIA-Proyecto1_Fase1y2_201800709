#ifndef MKFS_H
#define MKFS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <ctime>
#include <QList>

#include "structures.h"
#include "MKDisk.h"
#include "Mount.h"


using namespace std;

class MKFS
{
private:
    string id;
    string type;
    string ruta;
public:
    QList<Mount> *montajes;

    MKFS(string, string, QList<Mount>*);
    void Ejecutar();
    bool VerificarMontaje(string);
    void FormatFull(string);
    void FormatFast(string);
    Mount getMontaje(string);
    SuperBloque LLenar_SBlock( int, int, int, int, int);
    int Calcular_N(int);

};

#endif // MKFS_H
