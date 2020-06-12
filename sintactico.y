%{

#include <stdio.h>
#include <stdlib.h>
#include "nodoast.h"

extern int linea;


void yyerror(const char *s)
{
  printf("Error en linea: %u %s \n",linea,s);
}
extern char yytext; //
extern int yylex(void);
extern int linea;
extern FILE *yyin;
extern int yycolumn;

extern NodoAST *raiz = new NodoAST("INICIO");
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
%token <TEXT> TK_NUMBER
%token <TEXT> TK_NUMBER_NEGATIVO
%token <TEXT> TK_MKFS
%token <TEXT> TK_USR
%token <TEXT> TK_PWD
%token <TEXT> TK_LOGOUT
%token <TEXT> TK_MKGRP
%token <TEXT> TK_RMGRP
%token <TEXT> TK_MKUSR
%token <TEXT> TK_GRP
%token <TEXT> TK_RMUSR
%token <TEXT> TK_CHMOD
%token <TEXT> TK_UGO
%token <TEXT> TK_R
%token <TEXT> TK_MKFILE
%token <TEXT> TK_P
%token <TEXT> TK_CONT
%token <TEXT> TK_CAT
%token <TEXT> TK_FILE
%token <TEXT> TK_REM
%token <TEXT> TK_EDIT
%token <TEXT> TK_REN
%token <TEXT> TK_MKDIR
%token <TEXT> TK_CP
%token <TEXT> TK_MV
%token <TEXT> TK_DEST
%token <TEXT> TK_FIND
%token <TEXT> TK_CHOWN
%token <TEXT> TK_CHGRP
%token <TEXT> TK_PAUSE
%token <TEXT> TK_INODE
%token <TEXT> TK_JOURNALING
%token <TEXT> TK_BLOCK
%token <TEXT> TK_BM_INODE
%token <TEXT> TK_TREE
%token <TEXT> TK_SB
%token <TEXT> TK_LS
%token <TEXT> TK_RUTA
%token <TEXT> TK_CONTRA
%token <TEXT> TK_FILENAME

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
%type <TEXT> TIPO_NUMERO
%type <NODO> LISTAMKFS
%type <NODO> OPCION_MKFS
%type <NODO> LISTALOGIN
%type <NODO> OPCION_LOGIN
%type <TEXT> TIPO_PWD
%type <TEXT> TIPO_GRUPO
%type <NODO> LISTAMKUSR
%type <NODO> OPCION_MKUSR
%type <NODO> LISTACHMOD
%type <NODO> OPCION_CHMOD
%type <NODO> LISTAMKFILE
%type <NODO> OPCION_MKFILE
%type <NODO> LISTAEDIT
%type <NODO> OPCION_EDIT
%type <NODO> LISTAREN
%type <NODO> OPCION_REN
%type <NODO> LISTAMKDIR
%type <NODO> OPCION_MKDIR
%type <NODO> LISTACP
%type <NODO> OPCION_CP
%type <NODO> LISTAMV
%type <NODO> OPCION_MV
%type <NODO> LISTAFIND
%type <NODO> OPCION_FIND
%type <NODO> LISTACHOWN
%type <NODO> OPCION_CHOWN
%type <NODO> LISTACHGRP
%type <NODO> OPCION_CHGRP


%union{
    char *TEXT;
    class NodoAST *NODO;
}

%start INICIO;

%%

INICIO: INSTRUCCIONES { raiz=$$;}
;

INSTRUCCIONES: INSTRUCCIONES COMANDOS { $$ = $1; $$->agregarHijo(*$2); }
             | COMANDOS               { $$ = new NodoAST("INSTRUCCION"); $$->agregarHijo(*$1); }
;

COMANDOS: TK_MKDISK LISTAMKDISK                                 { NodoAST *n1 = new NodoAST("MkDisk"); n1->agregarHijo(*$2); $$ = n1; }
        | TK_RMDISK TK_GUION TK_PATH TK_IGUAL TIPO_RUTA         { NodoAST *n1 = new NodoAST("RmDisk"); NodoAST *n2 = new NodoAST("Path"); NodoAST *n5 = new NodoAST($5); n2->agregarHijo(*n5); n1->agregarHijo(*n2); $$ = n1; }
        | TK_FDISK LISTAFDISK                                   { NodoAST *n1 = new NodoAST("FDisk"); n1->agregarHijo(*$2); $$ = n1; }
        | TK_MOUNT LISTAMOUNT                                   { NodoAST *n1 = new NodoAST("Mount"); n1->agregarHijo(*$2); $$ = n1; }
        | TK_UNMOUNT TK_GUION TK_ID TK_IGUAL TK_IDENTIFICADOR   { NodoAST *n1 = new NodoAST("Unmount"); NodoAST *n2 = new NodoAST("ID"); NodoAST *n4 = new NodoAST($5); n2->agregarHijo(*n4); n1->agregarHijo(*n2); $$ = n1; }
        | TK_REP LISTAREP                                       { NodoAST *n1 = new NodoAST("Rep"); n1->agregarHijo(*$2); $$ = n1; }
        | TK_EXEC TK_GUION TK_PATH TIPO_RUTA                    { NodoAST *n1 = new NodoAST("Exec"); NodoAST *n2 = new NodoAST("Path"); NodoAST *n5 = new NodoAST($4); n2->agregarHijo(*n5); n1->agregarHijo(*n2); $$ = n1; }
        | TK_MKFS LISTAMKFS
        | TK_LOGIN LISTALOGIN
        | TK_LOGOUT
        | TK_MKGRP TK_GUION TK_NAME TK_IGUAL TIPO_GRUPO
        | TK_RMGRP TK_GUION TK_NAME TK_IGUAL TIPO_GRUPO
        | TK_MKUSR LISTAMKUSR
        | TK_RMUSR TK_GUION TK_USR TK_IGUAL TK_IDENTIFICADOR
        | TK_CHMOD LISTACHMOD
        | TK_MKFILE LISTAMKFILE
        | TK_CAT TK_GUION TK_FILE TK_IGUAL TIPO_RUTA
        | TK_REM TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
        | TK_EDIT LISTAEDIT
        | TK_REN LISTAREN
        | TK_MKDIR LISTAMKDIR
        | TK_CP LISTACP
        | TK_MV LISTAMV
        | TK_FIND LISTAFIND
        | TK_CHOWN LISTACHOWN
        | TK_CHGRP LISTACHGRP
        | TK_PAUSE
;

LISTAMKDISK: LISTAMKDISK OPCION_MKDISK  { $$ = $1; $$->agregarHijo(*$2); }
           | OPCION_MKDISK              { $$ = new NodoAST("Parametros"); $$->agregarHijo(*$1); }
;

OPCION_MKDISK: TK_GUION TK_SIZE TK_IGUAL TIPO_NUMERO  { NodoAST *n2 = new NodoAST("Size"); NodoAST *n4 = new NodoAST($4); n2->agregarHijo(*n4); $$ = n2; }
             | TK_GUION TK_UNIT TK_IGUAL TIPO_UNIT    { NodoAST *n2 = new NodoAST("Unit"); NodoAST *n4 = new NodoAST($4); n2->agregarHijo(*n4); $$ = n2; }
             | TK_GUION TK_PATH TK_IGUAL TIPO_RUTA    { NodoAST *n2 = new NodoAST("Ruta"); NodoAST *n4 = new NodoAST($4); n2->agregarHijo(*n4); $$ = n2; }
             | TK_GUION TK_FIT TK_IGUAL  TIPO_FIT     { NodoAST *n2 = new NodoAST("Fit");  NodoAST *n4 = new NodoAST($4); n2->agregarHijo(*n4); $$ = n2; }
;

TIPO_FIT: TK_BESTFIT    { $$ = "bf"; }
        | TK_FIRSTFIT   { $$ = "ff"; }
        | TK_WORSTFIT   { $$ = "wf"; }
;

TIPO_UNIT: TK_BYTES     { $$ = "b"; }
         | TK_KYLOBYTES { $$ = "k"; }
         | TK_MEGABYTES { $$ = "m"; }
;

LISTAFDISK: LISTAFDISK OPCION_FDISK { $$ = $1; $$->agregarHijo(*$2); }
          | OPCION_FDISK            { $$ = new NodoAST("Parametros"); $$->agregarHijo(*$1); }
;

OPCION_FDISK: TK_GUION TK_SIZE TK_IGUAL TIPO_NUMERO         { NodoAST *n1 = new NodoAST("Size"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_UNIT TK_IGUAL TIPO_UNIT           { NodoAST *n1 = new NodoAST("Unit"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_PATH TK_IGUAL TIPO_RUTA           { NodoAST *n1 = new NodoAST("Path"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_TYPE TK_IGUAL TIPO_PARTICION      { NodoAST *n1 = new NodoAST("Type"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_FIT TK_IGUAL TIPO_FIT             { NodoAST *n1 = new NodoAST("Fit"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_DELETE TK_IGUAL TIPO_DELETE       { NodoAST *n1 = new NodoAST("Delete"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_NAME TK_IGUAL TK_IDENTIFICADOR    { NodoAST *n1 = new NodoAST("Name"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_ADD TK_IGUAL TIPO_NUMERO          { NodoAST *n1 = new NodoAST("Add"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
;

TIPO_DELETE: TK_FAST    { $$ = "Fast"; }
           | TK_FULL    { $$ = "Full"; }
;

TIPO_PARTICION: TK_PRIMARIA     { $$ = "Primaria"; }
              | TK_EXTENDIDA    { $$ = "Extendida"; }
              | TK_LOGICA       { $$ = "Logica"; }
;

LISTAMOUNT: LISTAMOUNT OPCION_MOUNT { $$ = $1; $$->agregarHijo(*$2); }
          | OPCION_MOUNT            { $$ = new NodoAST("Parametros"); $$->agregarHijo(*$1); }
;

OPCION_MOUNT: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA           { NodoAST *n1 = new NodoAST("Path"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
            | TK_GUION TK_NAME TK_IGUAL TK_IDENTIFICADOR    { NodoAST *n1 = new NodoAST("Name"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
;

LISTAREP: LISTAREP OPCION_REP   { $$ = $1; $$->agregarHijo(*$2); }
        | OPCION_REP            { $$ = new NodoAST("Parametros"); $$->agregarHijo(*$1); }
;

OPCION_REP: TK_GUION TK_NAME TK_IGUAL TIPO_REP          { NodoAST *n1 = new NodoAST("Name"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
          | TK_GUION TK_PATH TK_IGUAL TIPO_RUTA         { NodoAST *n1 = new NodoAST("Path"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
          | TK_GUION TK_ID TK_IGUAL TK_IDENTIFICADOR    { NodoAST *n1 = new NodoAST("ID"); NodoAST *n4 = new NodoAST($4); n1->agregarHijo(*n4); $$ = n1; }
;

TIPO_REP: TK_MBR    { $$ = "MBR";}
        | TK_DISK   { $$ = "Disk"; }
;

TIPO_RUTA: TK_RUTA              { $$ = $1; }
         | TK_IDENTIFICADOR     { $$ = $1; }
         | TK_CADENA            { $$ = $1; }
;

TIPO_NUMERO: TK_NUMBER          { $$ = $1; }
           | TK_NUMBER_NEGATIVO { $$ = $1; }
;

LISTAMKFS: LISTAMKFS OPCION_MKFS 
         | OPCION_MKFS
;

OPCION_MKFS: TK_GUION TK_ID TK_IGUAL TK_IDENTIFICADOR
           | TK_GUION TK_TYPE TK_IGUAL TIPO_DELETE
;

LISTALOGIN: LISTALOGIN OPCION_LOGIN
          | OPCION_LOGIN
;

OPCION_LOGIN: TK_GUION TK_USR TK_IGUAL TK_IDENTIFICADOR
            | TK_GUION TK_PWD TK_IGUAL TIPO_PWD
            | TK_GUION TK_ID TK_IGUAL TK_IDENTIFICADOR
;

TIPO_PWD: TK_CADENA
        | TK_CONTRA
        | TK_IDENTIFICADOR
;

TIPO_GRUPO: TK_CADENA
          | TK_IDENTIFICADOR
;

LISTAMKUSR: LISTAMKUSR OPCION_MKUSR
          | OPCION_MKUSR
;

OPCION_MKUSR: TK_GUION TK_USR TK_IGUAL TK_IDENTIFICADOR 
            | TK_GUION TK_PWD TK_IGUAL TIPO_PWD
            | TK_GUION TK_GRP TK_IGUAL TIPO_GRUPO
;

LISTACHMOD: LISTACHMOD OPCION_CHMOD
          | OPCION_CHMOD
;

OPCION_CHMOD: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
            | TK_GUION TK_UGO TK_IGUAL TK_NUMBER
            | TK_GUION TK_R 
;

LISTAMKFILE: LISTAMKFILE OPCION_MKFILE
          | OPCION_MKFILE
;

OPCION_MKFILE: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
             | TK_GUION TK_SIZE TK_IGUAL TK_NUMBER
             | TK_GUION TK_CONT TK_IGUAL TIPO_RUTA
             | TK_GUION TK_P 
;

LISTAEDIT: LISTAEDIT OPCION_EDIT
         | OPCION_EDIT
;

OPCION_EDIT: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
           | TK_GUION TK_CONT TK_IGUAL TIPO_RUTA
;

LISTAREN: LISTAREN OPCION_REN
        | OPCION_REN
;

OPCION_REN: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
          | TK_GUION TK_NAME TK_IGUAL TK_FILENAME
;

LISTAMKDIR: LISTAMKDIR OPCION_MKDIR
          | OPCION_MKDIR
;

OPCION_MKDIR: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
            | TK_GUION TK_P 
;

LISTACP: LISTACP OPCION_CP
       | OPCION_CP
;

OPCION_CP: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
         | TK_GUION TK_DEST TK_IGUAL TIPO_RUTA
;

LISTAMV: LISTAMV OPCION_MV
       | OPCION_MV
;

OPCION_MV: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
         | TK_GUION TK_DEST TK_IGUAL TIPO_RUTA
;

LISTAFIND: LISTAFIND OPCION_FIND
         | OPCION_FIND
;

OPCION_FIND: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
           | TK_GUION TK_NAME TK_IGUAL TK_FILENAME
;

LISTACHOWN: LISTACHOWN OPCION_CHOWN
          | OPCION_CHOWN
;

OPCION_CHOWN: TK_GUION TK_PATH TK_IGUAL TIPO_RUTA
            | TK_GUION TK_USR TK_IGUAL TK_IDENTIFICADOR
            | TK_GUION TK_R 
;

LISTACHGRP: LISTACHGRP OPCION_CHGRP
          | OPCION_CHGRP
;

OPCION_CHGRP: TK_GUION TK_GRP TK_IGUAL TIPO_GRUPO
            | TK_GUION TK_USR TK_IGUAL TK_IDENTIFICADOR
;


%%
