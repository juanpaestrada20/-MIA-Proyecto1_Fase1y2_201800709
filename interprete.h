#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <nodoast.h>
#include <fstream>
#include <QString>
#include <QChar>
#include <iostream>
#include <structures.h>


class Interprete
{
private:
    NodoAST *raiz;
    int size;
    QString fitPredeterminadoM; // Predeterminado par MkDisk
    QString fitPredeterminadoF; //Predeterminado par FDisk
    QChar unitPredeterminada;
    QString path;
    QChar typePredeterminado;
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
    void Opciones_Parametro(NodoAST *raiz);
    char getFitChar(QString fit);
};

#endif // INTERPRETE_H
