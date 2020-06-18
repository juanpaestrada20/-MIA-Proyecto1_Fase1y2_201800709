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
    QList<Mount> *montajes;
    Rep(QString name, QString path, string id, QList<Mount> *montajes);
    void Analizar_Rep(int tamanio);

    void Hacer_Reporte();

    void ReporteDisk(string ruta);
    void ReporteMBR(string ruta);
    void ReporteEBR(string ruta);
    void VerificarDirectorio();
    string getRuta();
};



#endif // REP_H
