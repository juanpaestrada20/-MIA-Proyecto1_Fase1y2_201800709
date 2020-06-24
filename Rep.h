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

extern Sesion daLoguer;

class Rep{
public:
    string id;
    QString name;
    QString path;
    string ext;
    string ruta;
    int posMontada;
    QList<Mount> *montajes;
    Rep(QString , QString, string, QList<Mount> *,string);
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
    string getExtension(QString);
    void ReporteBlock(string);
    void graficarBloques(string , string, string, int, int, int);
    void ReporteBM_Inode(string);
    void reporteBM(string, QString, int, int);
    void ReporteBM_Block(string);
    void ReporteTree(string);
    void graficarTree(string, QString, string, int);
    void ReporteSB(string);
    void graficarSuper(string, QString, string, int);
    void graficarFILE(string, QString, string, QString,int,int);
    void ReporteLS(string);
    void graficarPermisos(FILE*, FILE*, int, Usuario);
    int buscarCarpetaArchivo(FILE*, char*);
    int byteInodoBloque(FILE*, int, char);
    Usuario getUsuario(string, int);
    int buscarGrupo(string);

};



#endif // REP_H
