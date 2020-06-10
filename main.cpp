#include <QDebug>
#include <QDateTime>
#include <QString>
#include <string>
#include <iostream>
#include <istream>
#include <stdio.h>
#include <cctype>
#include <fstream>
#include <math.h>

#include "lexico.h"
#include "sintactico.h"
#include "Interprete.h"


extern int yylex(void);
extern char *yytext;
extern int linea;
extern FILE *yyin;
extern int yyparse(void);
extern NodoAST *raiz;

using namespace std;

void menu();
string toLower(string);
void parser(string);

int main()
{

    menu();

}

void menu(){
    string entrada = "";
    system("clear");
    cout << "MANEJO E IMPLEMENTACIO DE ARCHIVOS" << endl;
    while(entrada != "exit"){
        cout << "201800709@MIA: ~$ ";
        getline(cin, entrada);
        // por si entra el comando de exec
        if(toLower(entrada.substr(0,4)) == "exec"){
            if(entrada.find("-path") == string::npos){
                printf("Debe incluir el parametro path\n");

            }else{
                string ruta = entrada.substr(entrada.find("=")+1, entrada.length() - 1);
                if(ruta[0] == '"'){
                    ruta = ruta.substr(1, ruta.length()-2);
                }
                ifstream fi(ruta);
                string buff;
                string textoAnalizar;
                while(getline(fi, buff)){
                    textoAnalizar += buff + "\n";
                }
                parser(textoAnalizar);
            }
        }else if(entrada == "exit"){
            return;
        }else{
            parser(entrada);
        }

    }
}

void parser(string entrada){
    YY_BUFFER_STATE bufferState = yy_scan_string(entrada.c_str());
    if(yyparse()==0){
        qDebug() << "Analisis Completado";
        Interprete *interprete = new Interprete(raiz);
        interprete->ejecutar();
    }else{
        qDebug() << "Analisis no  se pudo completar";
    }
    yy_delete_buffer(bufferState);
}

string toLower(string entrada){
    string out;
    if(entrada == "" || entrada == "\n"){
        return entrada;
    }
    foreach (char c, entrada) {
        out += tolower(c);
    }
    return out;
}
