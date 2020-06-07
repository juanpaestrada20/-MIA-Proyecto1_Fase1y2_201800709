#include "interprete.h"

Interprete::Interprete(NodoAST *raiz)
{
    this->raiz = raiz;
    this->size = 1;
    this->fitPredeterminadoM = "ff"; // Predeterminado par MkDisk
    this->fitPredeterminadoF = "wf"; //Predeterminado par FDisk
    this->unitPredeterminada = 'm';
    this->path = "";
    this->typePredeterminado = 'p';
    this->deletePredeterminado = "";
    this->name = "";
    this->add = 0;
    this->ID = "";
    this->error = false;
    this->opcion_fdisk = 0;
}

void Interprete::ejecutar(){
    Recorrer_Arbol(&raiz->hijos[0]);
}

void Interprete::Recorrer_Arbol(NodoAST *raiz){
    QString tipoComando = raiz->valor;

    if(tipoComando == "MkDisk"){
        /* Se obtiene la cantidad de parametros del comando ingresado */
        int cantHijos = raiz->hijos.count();
        for(int i = 0; i < cantHijos; i++){
            if(error){
                break;
            }
            Opciones_Parametro(&raiz->hijos[i]);
        }


    } else if(tipoComando == "RmDisk"){

    } else if(tipoComando == "FDisk"){

    } else if(tipoComando == "Mount"){

    } else if(tipoComando == "Unmount"){

    } else if(tipoComando == "Rep"){

    } else if(tipoComando == "Exec"){

    }
}

void Interprete::Opciones_Parametro(NodoAST *raiz){

}

char Interprete::getFitChar(QString fit){

}
