#ifndef MKFS_H
#define MKFS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <ctime>
#include <QList>

#include "structures.h"
#include "MKDisk.h"
#include "Mount.h"

using namespace std;

class MKFS
{
private:
    string id;
    string type;
    string ruta;
public:
    QList<Mount> *montajes;

    MKFS(string, string, QList<Mount>*);
    void Ejecutar();
    bool VerificarMontaje(string);
    void FormatFull(string);
    void FormatFast(string);
    Mount getMontaje(string);
    void LLenar_SBlock(SuperBloque*, int, int);
    void Crear_Raiz(string, SuperBloque, Mount);
    int Calcular_N(int);
    int Calculo_Posicion_Inodo(SuperBloque, int);
    int Calculo_Posicion_Block(SuperBloque, int);
    void Modificar_Count_I(SuperBloque*, int, FILE*, int, int);
    void Modificar_Count_B(SuperBloque*, int, FILE*, int, int);
    void Marcar_BitMap_I(int, int, FILE*, char, int);
    void Marcar_BitMap_B(int, int, FILE*, char, int);

    int BM_Inodo_Disponible(FILE*,int,int);
    int BM_Bloque_Disponible(FILE*,int,int);

    void Modificar_Inodo(FILE*, Inodo, int, int);

};

#endif // MKFS_H
