%{

#include <stdio.h>
#include <stdlib.h>
#include "nodoast.h"

extern int linea;


void yyerror(const char *s)
{
  printf("%s \n",s);
}
extern char yytext; //
extern int yylex(void);
extern int linea;
extern FILE *yyin;
extern int yycolumn;

extern NodoAST *raiz = new NodoAST(0,0,"INICIO");
%}
%locations
%define parse.trace
%define parse.error verbose

%token <TEXT> TK_MKDISK
%token <TEXT> TK_RMDISK
%token <TEXT> TK_FDISK
%token <TEXT> TK_MOUNT
%token <TEXT> TK_UNMOUNT
%token <TEXT> TK_REP
%token <TEXT> TK_SIZE
%token <TEXT> TK_FIT
%token <TEXT> TK_UNIT
%token <TEXT> TK_PATH
%token <TEXT> TK_TYPE
%token <TEXT> TK_DELETE
%token <TEXT> TK_NAME
%token <TEXT> TK_ADD
%token <TEXT> TK_ID
%token <TEXT> TK_EXEC
%token <TEXT> TK_BESTFIT
%token <TEXT> TK_FIRSTFIT
%token <TEXT> TK_WORSTFIT
%token <TEXT> TK_KYLOBYTES
%token <TEXT> TK_MEGABYTES
%token <TEXT> TK_BYTES
%token <TEXT> TK_PRIMARIA
%token <TEXT> TK_EXTENDIDA
%token <TEXT> TK_LOGICA
%token <TEXT> TK_FAST
%token <TEXT> TK_FULL
%token <TEXT> TK_MBR
%token <TEXT> TK_DISK
%token TK_GUION
%token TK_IGUAL
%token <TEXT> TK_CADENA
%token <TEXT> TK_RUTA
%token <TEXT> TK_IDENTIFICADOR
%token <NUMERO> TK_NUMBER

%type <NODO> INICIO
%type <NODO> INSTRUCCIONES
%type <NODO> COMANDOS
%type <NODO> LISTAMKDISK
%type <NODO> OPCION_MKDISK
%type <TEXT> TIPO_FIT
%type <TEXT> TIPO_UNIT
%type <NODO> LISTAFDISK
%type <NODO> OPCION_FDISK
%type <TEXT> TIPO_DELETE
%type <TEXT> TIPO_PARTICION
%type <NODO> LISTAMOUNT
%type <NODO> OPCION_MOUNT
%type <NODO> LISTAREP
%type <NODO> OPCION_REP
%type <TEXT> TIPO_REP
%type <TEXT> TIPO_RUTA

%union{
    char TEXT[256];
    int NUMERO;
    class NodoAST *NODO;
}

%start INICIO;

%%

INICIO: INSTRUCCIONES { raiz->agregarHijo(*$1); }
;

INSTRUCCIONES: INSTRUCCIONES COMANDOS { $$ = $1; $$->agregarHijo(*$2); }
             | COMANDOS               { $$->agregarHijo(*$1); }
;

COMANDOS: TK_MKDISK LISTAMKDISK                                 { NodoAST *n1 = new NodoAST("MkDisk"); n1->agregarHijo(*$2); $$ = n1; }
        | TK_RMDISK TK_GUION TK_PATH TK_IGUAL TIPO_RUTA         { NodoAST *n1 = new NodoAST("RmDisk"); NodoAST *n2 = new NodoAST("Path"); n2->agregarHijo(*$5); $$ = n1; }
        | TK_FDISK LISTAFDISK                                   { NodoAST *n1 = new NodoAST("FDisk"); n1->agregarHijo(*$2); $$ = n1; }
        | TK_MOUNT LISTAMOUNT                                   { NodoAST *n1 = new NodoAST("Mount"); n1->agregarHijo(*$2); $$ = n1; }
        | TK_UNMOUNT TK_GUION TK_ID TK_IGUAL TK_IDENTIFICADOR   { NodoAST *n1 = new NodoAST("Unmount"); NodoAST *n2 = new NodoAST(linea, yycolumn, "ID"); n2->agregarHijo(*$5); $$ = n1; }
        | TK_REP LISTAREP                                       { NodoAST *n1 = new NodoAST("Rep"); n1->agregarHijo(*$2); $$ = n1; }
        | TK_EXEC TK_GUION TK_PATH TIPO_RUTA                    { NodoAST *n1 = new NodoAST("Exec"); NodoAST *n2 = new NodoAST("Path"); n2->agregarHijo(*$4); $$ = n1; }
;

LISTAMKDISK: LISTAMKDISK OPCION_MKDISK  { $$ = $1; $$.agregarHijo(*$2); }
           | OPCION_MKDISK              { $$.agregarHijo(*$1); }
;

OPCION_MKDISK: TK_GUION TK_SIZE TK_IGUAL TK_NUMBER  { NodoAST *n2 = new NodoAST("Size"); NodoAST *n4 = new NodoAST($4); n2->agregarHijo(*n4); $$ = n2; }
             | TK_GUION TK_UNIT TK_IGUAL TIPO_UNIT  { NodoAST *n2 = new NodoAST("Unit"); NodoAST *n4 = new NodoAST($4); n2->agregarHijo(*n4); $$ = n2; }
             | TK_GUION TK_PATH TK_IGUAL TIPO_RUTA  { NodoAST *n2 = new NodoAST("Ruta"); NodoAST *n4 = new NodoAST($4); n2->agregarHijo(*n4); $$ = n2; }
             | TK_GUION TK_FIT TK_IGUAL  TIPO_FIT   { NodoAST *n2 = new NodoAST("Fit");  NodoAST *n4 = new NodoAST($4); n2->agregarHijo(*n4); $$ = n2; }
;

TIPO_FIT: TK_BESTFIT    { $$ = "BF"; }
        | TK_FIRSTFIT   { $$ = "FF"; }
        | TK_WORSTFIT   { $$ = "WF"; }
;

TIPO_UNIT: TK_BYTES     { $$ = "B"; }
         | TK_KYLOBYTES { $$ = "KB"; }
         | TK_MEGABYTES { $$ = "MB"; }
;

LISTAFDISK: LISTAFDISK OPCION_FDISK { $$ = $1; $$->agregarHijo(*$2); }
          | OPCION_FDISK            { $$->agregarHijo(*$1); }
;

OPCION_FDISK: TK_GUION TK_SIZE TK_IGUAL TK_NUMBER           { NoodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_UNIT TK_IGUAL TIPO_UNIT           { NoodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*$4); $$ = n1; }
            | TK_GUION TK_PATH TK_IGUAL TIPO_RUTA           { NoodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*$4); $$ = n1; }
            | TK_GUION TK_TYPE TK_IGUAL TIPO_PARTICION      { NoodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*$4); $$ = n1; }
            | TK_GUION TK_FIT TK_IGUAL TIPO_FIT             { NoodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*$4); $$ = n1; }
            | TK_GUION TK_DELETE TK_IGUAL TIPO_DELETE       { NoodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*$4); $$ = n1; }
            | TK_GUION TK_NAME TK_IGUAL TK_IDENTIFICADOR    { NoodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_ADD TK_IGUAL TK_NUMBER            { NoodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
;

TIPO_DELETE: TK_FAST    { $$ = "Fast"; }
           | TK_FULL    { $$ = "Full"; }
;

TIPO_PARTICION: TK_PRIMARIA     { $$ = "Primaria"; }
              | TK_EXTENDIDA    { $$ = "Extendida"; }
              | TK_LOGICA       { $$ = "Logica"; }
;

LISTAMOUNT: LISTAMOUNT OPCION_MOUNT { $$ = $1; $$->agregarHijo(*$2); }
          | OPCION_MOUNT            { $$->agregarHijo(*$1); }
;

OPCION_MOUNT: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA           { NodoAST *n1 = new NodoAST("Path"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_NAME TK_IGUAL TK_IDENTIFICADOR    { NodoAST *n1 = new NodoAST("Name"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
;

LISTAREP: OPCION_REP LISTAREP   { $$ = $1; $$->agregarHijo(*$2); }
        | OPCION_REP            { $$->agregarHijo(*$1); }
;

OPCION_REP: TK_GUION TK_NAME TK_IGUAL TIPO_REP          { NodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarhijo(*n4); $$ = n1; }
          | TK_GUION TK_PATH TK_IGUAL TIPO_RUTA         { NodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarhijo(*n4); $$ = n1; }
          | TK_GUION TK_ID TK_IGUAL TK_IDENTIFICADOR    { NodoAST *n1 = new NodoAST(""); NodoAST *n4 = new NodoAST($4); n1->agregarhijo(*n4); $$ = n1; }
;

TIPO_REP: TK_MBR    { $$ = "MBR" }
        | TK_DISK   { $$ = "Disk" }
;

TIPO_RUTA: TK_RUTA      { $$ = $1; }
         | TK_CADENA    { $$ = $1; }
;

%%
