#ifndef REP_H
#define REP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "structures.h"
#include "MKDisk.h"
#include "Mount.h"

class Rep{
public:
    string id;
    QString name;
    QString path;
    int posMontada;
    QList<Mount> *montajes;
    Rep(QString , QString, string, QList<Mount> *);
    void Analizar_Rep(int );

    void Hacer_Reporte();

    void ReporteDisk(string );
    void ReporteMBR(string );
    void ReporteEBR(string);
    void ReporteJournaling(string);
    void VerificarDirectorio();
    string getRuta();
    void graficarJournaling(string, string, string, int);
    void graficarInodos(string , string , string ,int ,int ,int );
    void ReporteInodos(string);
    void ReporteFile(string);
    int buscarCarpetaArchivo(FILE*, char*);
};



#endif // REP_H
