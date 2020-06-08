#ifndef FDISK_H
#define FDISK_H
#include <QString>
#include <QChar>
#include <stdio.h>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <QStringList>
#include <string>
#include "structures.h"

class FDisk
{
public:
    int size;
    char unit;
    QString path;
    QString type;
    QString fit;
    QString eliminar;
    QString name;
    int add;
    int opcion_exe;
    FDisk(int size, char unit, QString path, QString type, QString fit, QString eliminar, QString name, int add, int opcion);
    void Ejecutar();
    int asignar_size(char type);
    int buscar_primer_EBR(MBR disco_actual);
    int Espacio_disponible(MBR MBR_actual);
    void Estado_Fdisk();

    void Validar_Fdisk();
    void Crear_Particion(bool raid);
    void Eliminar_Particion( );
    void Modificar_Particion( );

    int Calcular_Espacio();
    int Calcular_Espacio_Add();
    Partition LLenar_Particion(int inicio);
    bool Nombre_noRepetido(MBR mbr, FILE* disco);
    void Particion_Add(bool raid);
    void Particion_Delete(bool raid);
    Partition Vaciar_Particion();


    bool Existe_Extendida(MBR mbr);
    EBR LLenar_EBR(int inicio);

};

#endif // FDISK_H
