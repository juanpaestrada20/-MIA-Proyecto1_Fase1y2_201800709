#ifndef MKUSR_H
#define MKUSR_H
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

class MKUSR
{
private:
    string user;
    string password;
    string grupo;
public:
    MKUSR(string, string, string);
    void Ejecutar();
    int buscarGrupo(string);
    bool buscarUsuario(string);
    void agregarUsuario(string);
    void agregarUsuariosTexto(string);
    void guardarJournal(char*, int, int, char*);
    int getIdUser();
    int buscarBit(FILE*, char, char);
};

#endif // MKUSR_H
