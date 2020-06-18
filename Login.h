#ifndef LOGIN_H
#define LOGIN_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <iostream>
#include <QList>
#include <math.h>
#include "Mount.h"
#include "structures.h"

using namespace std;
extern QList<Mount> *montajes;
extern Sesion daLoguer;
extern bool login;
class Login
{
private:
    string user;
    string pass;
    string id;
    string ruta;
    string name;
    Mount montaje = *new Mount("", "", "", 0);
public:
    Login(string, string, string);
    void Ejecutar();
    bool GetRuta(string);
    bool ComprobarLogin(string, string, string);
    void LeerArchivo(FILE*, SuperBloque, int, char*, string);
    void Eliminar_Espacios(char*);
    int Get_Id_Group(string);
    int Existe_Archivo(FILE*,SuperBloque, int,string);
    int Buscar_Inodo(FILE *, SuperBloque , int , string ,int ,int );
    int Buscar_BCarpeta(FILE *, SuperBloque , int ,  string ,int ,int );
    int Buscar_BApuntador(FILE *, SuperBloque , int , string, int , int );
    int Calculo_Posicion_Block(SuperBloque , int );
    int Calcular_N(int);
    int Calculo_Posicion_Inodo(SuperBloque , int );
    void Modificar_Inodo(FILE* , Inodo , int , int );
    string Leer_Inodo_Archivo( FILE* , SuperBloque ,int  , int , char*);
    bool Leer_BApuntador_Archivo(FILE *disco, SuperBloque ,int , char* ,int ,int* );
    bool Leer_BArchivo(FILE* , SuperBloque , char* , int , int* );
};

#endif // LOGIN_H
