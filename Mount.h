
#ifndef MOUNT_H
#define MOUNT_H

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "MKDisk.h"
#include "structures.h"
#include "FDisk.h"
#include <QList>
#include <string>

class Mount
{
public:
    string path;
    string name;
    string id;
    char letra;
    int num;
    QList<Mount> *montajes;
    int inicio;
    int size;
    char type;
    Mount(string name, string path, string id, QList<Mount> *montajes);
    void Ejecutar();
    bool Nombre_noRepetido(MBR mbr, FILE *disco);
    Mount* Get_Mount(string path);
    string setID(string letra, int num);
    string getLetra();
    bool VerificarMontaje();
};


#endif // MOUNT_H
