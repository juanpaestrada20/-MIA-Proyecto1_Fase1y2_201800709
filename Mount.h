
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
    QString path;
    QString name;
    string id;
    char letra;
    int num;
    QList<Mount> *montajes;
    Mount(QString name, QString path, string id, QList<Mount> *montajes);
    void Ejecutar();
    bool Nombre_noRepetido(MBR mbr, FILE *disco);
    Mount* Get_Mount(QString path);
    string setID(QString letra, int num);
    QString getLetra();
    bool VerificarMontaje();
};


#endif // MOUNT_H
