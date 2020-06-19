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
    int Calcular_N(int);
};

#endif // LOGIN_H
