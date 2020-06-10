#include "nodoast.h"
NodoAST::NodoAST(QString valor)
{
    this->valor = valor;
}

void NodoAST::agregarHijo(NodoAST nodo){
    this->hijos.append(nodo);
}
