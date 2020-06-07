#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <nodoast.h>
#include <fstream>
#include <QString>
#include <QChar>
#include <iostream>
#include <structures.h>
#include "MKDisk.h"


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
};

#endif // INTERPRETE_H
