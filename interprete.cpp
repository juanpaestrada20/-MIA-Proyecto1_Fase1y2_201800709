#include "interprete.h"
#include "structures.h"
#include "nodoast.h"
#include <stdlib.h>
#include <stdio.h>
#include <QString>
#include <QDateTime>
#include "MKDisk.h"

using namespace std;

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
        Opciones_Parametro(&raiz->hijos[0], 0);
        if(!this->error){
            int tam = this->size;
            char fit[2];
            fit[0] = this->fitPredeterminadoM.toLower().toStdString()[0];
            fit[1] = this->fitPredeterminadoM.toLower().toStdString()[1];
            char unidad = this->unitPredeterminada.toLower().toStdString()[0];
            QString ruta = this->path;

            MKDisk *DiscoNuevo = new MKDisk(tam, fit, unidad, ruta);
            DiscoNuevo->Ejecutar();
        }

    } else if(tipoComando == "RmDisk"){

    } else if(tipoComando == "FDisk"){

    } else if(tipoComando == "Mount"){

    } else if(tipoComando == "Unmount"){

    } else if(tipoComando == "Rep"){

    } else if(tipoComando == "Exec"){

    }
}

void Interprete::Opciones_Parametro(NodoAST *raiz, int tipo){
    switch (tipo) {
    case 0:
    {
        int cantParametros = raiz->hijos.count();
        int size = 0;
        char unit = 'n';
        QString path = "";
        char fit[] = "no";
        QString parametro;

        for(int i = 0; i < cantParametros; i++){
            parametro=raiz->hijos[i].valor;
         if(parametro == "Size"){
             size = raiz->hijos[i].hijos[0].valor.toInt();
         } else if(parametro == "Unit"){
             unit = raiz->hijos[i].hijos[0].valor.toStdString()[0];
         } else if(parametro == "Ruta"){
             path = raiz->hijos[i].hijos[0].valor;
         } else if(parametro == "Fit"){
             fit[0] = raiz->hijos[i].hijos[0].valor.toLower().toStdString()[0];
             fit[1] = raiz->hijos[i].hijos[0].valor.toLower().toStdString()[1];
         }
        }
        // si hay algun parametro obligatorio faltante
        if (size > 0){
            this->size = size;
        }else{
            this->error = true;
            printf("Tamaño de Disco no permitido\n");
        }
        if(path != ""){
            this->path = path;
        }else{
            this->error = true;
            printf("Ubicacion para crear disco no especificada\n");
        }
        if(unit=='n'){
            this->unitPredeterminada = "m";
        }else{
            if(unit=='k'){
                this->unitPredeterminada = "k";
            }else if(unit=='m'){
                this->unitPredeterminada = "m";
            }else if(unit=='b'){
                this->unitPredeterminada = "b";
            }else{
                this->error = true;
                printf("No se reconoce el tipo de unidad ingresado\n");
            }
        }
        if(fit[0]!='n' && fit[1]!='o'){
            if(fit[0]=='f' && fit[1]=='f'){
                fitPredeterminadoM = fit[0] +fit[1];
            }
            else if(fit[0]=='w' && fit[1]=='f'){
                fitPredeterminadoM = fit[0] +fit[1];
            }
            else if(fit[0]=='b' && fit[1]=='f'){
                fitPredeterminadoM = fit[0] +fit[1];
            }else{
                this->error= true;
                printf("No se reconoce el fit\n");
            }
        }
        break;
    }
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    }
}

char Interprete::getFitChar(QString fit){

}
