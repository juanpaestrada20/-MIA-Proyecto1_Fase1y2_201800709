#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <nodoast.h>
#include <fstream>
#include <QString>
#include <QChar>
#include <iostream>
#include <structures.h>
#include "MKDisk.h"
#include "structures.h"
#include "nodoast.h"
#include <stdlib.h>
#include <stdio.h>
#include <QString>
#include <QDateTime>
#include <QList>
#include "RMDisk.h"
#include "FDisk.h"
#include "Mount.h"
#include "Rep.h"
#include "MKFS.h"

extern     QList<Mount> *montajes;

class Interprete
{
private:
    NodoAST *raiz;
    int size;
    QString fitPredeterminadoM; // Predeterminado par MkDisk
    QString fitPredeterminadoF; //Predeterminado par FDisk
    QString unitPredeterminada;
    QString path;
    QString typePredeterminado;
    QString deletePredeterminado;
    QString name;
    string user;
    string pass;
    string grupo;
    int ugo;
    bool recursivo;
    bool carpetaPadre;
    string cont;
    string dest;

    int add;
    QString ID;
    bool error;
    int opcion_fdisk;

public:

    Interprete(NodoAST *raiz);
    void ejecutar();
    void Recorrer_Arbol(NodoAST *raiz);
    void Opciones_Parametro(NodoAST *raiz, int tipo);
    char getFitChar(QString fit);
    void restorePred();
    void EliminarMount(QString id);
};

#endif // INTERPRETE_H
