#ifndef NODOAST_H
#define NODOAST_H

#include <QList>
#include <QString>

class NodoAST
{
public:
    QString valor;
    QList<NodoAST> hijos;
    NodoAST(QString);
    void agregarHijo(NodoAST nodo);
};

#endif // NODOAST_H
