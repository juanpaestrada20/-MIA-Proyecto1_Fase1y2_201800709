#ifndef RMDISK_H
#define RMDISK_H

#include <QString>
#include <iostream>
#include <stdlib.h>
#include <fstream>

class RMDisk
{
public:
    QString path;
    RMDisk(QString path);
    void Ejecutar();
};

#endif // RMDISK_H
