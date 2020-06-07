#ifndef MKDISK_H
#define MKDISK_H

#include <QString>
#include <QChar>
#include <stdio.h>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "structures.h"

class MKDisk
{
public:
    int size;
    char fit[2];
    char unit;
    QString path;
    QString pathRAID;
    MKDisk(int size, char fit[], char unit, QString path);
    void Ejecutar();
    void VerificarDirectorio(char* ruta);
    MBR MasterBootRecord;
};

#endif // MKDISK_H
