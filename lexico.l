%option case-insensitive
%option noline
%option noyywrap
%option header-file="lexico.h"
%option outfile="lexico.cpp"
%option nounput
%{

#include <stdio.h>
#include <stdlib.h>
#include "sintactico.h"

int yycolumn = 1;

int linea = 1;
%}


COMENTARIO "#".*
DIG [0-9]
NUM {DIG}+
NUMNEGATIVO [-][0-9]+
IDENT [a-zA-Z_][a-zA-Z_0-9]*
RUTA "/"[^\n\t\r" "]+
CADENA \"[^\"]*\"
CONTRA [a-zA-Z_0-9]+
NOMBRE_FILE [{IDENT}][.][a-zA-Z]

%%

[\t\r\f" "]+		        {}

{COMENTARIO}		        {}

"mkdisk"			        { sscanf(yytext, "%s", yylval.TEXT); return (TK_MKDISK); }
"rmdisk"			        { sscanf(yytext, "%s", yylval.TEXT); return (TK_RMDISK); }
"fdisk"			            { sscanf(yytext, "%s", yylval.TEXT); return (TK_FDISK); }
"mount"			            { sscanf(yytext, "%s", yylval.TEXT); return (TK_MOUNT); }
"unmount"			        { sscanf(yytext, "%s", yylval.TEXT); return (TK_UNMOUNT); }
"rep"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_REP); }
"size"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_SIZE); }
"fit"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_FIT); }
"bf"                        { sscanf(yytext, "%s", yylval.TEXT); return (TK_BESTFIT); }
"ff"                        { sscanf(yytext, "%s", yylval.TEXT); return (TK_FIRSTFIT); }
"wf"                        { sscanf(yytext, "%s", yylval.TEXT); return (TK_WORSTFIT); }
"unit"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_UNIT); }
"k"                         { sscanf(yytext, "%s", yylval.TEXT); return (TK_KYLOBYTES); }
"m"                         { sscanf(yytext, "%s", yylval.TEXT); return (TK_MEGABYTES); }
"b"                         { sscanf(yytext, "%s", yylval.TEXT); return (TK_BYTES); }
"path"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_PATH); }
"type"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_TYPE); }
"p"                         { sscanf(yytext, "%s", yylval.TEXT); return (TK_PRIMARIA); }
"e"                         { sscanf(yytext, "%s", yylval.TEXT); return (TK_EXTENDIDA); }
"l"                         { sscanf(yytext, "%s", yylval.TEXT); return (TK_LOGICA); }
"delete"			        { sscanf(yytext, "%s", yylval.TEXT); return (TK_DELETE); }
"fast"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_FAST); }
"full"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_FULL); }
"name"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_NAME); }
"add"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_ADD); }
"id"				        { sscanf(yytext, "%s", yylval.TEXT); return (TK_ID); }
"exec"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_EXEC); }
"mbr"                       { sscanf(yytext, "%s", yylval.TEXT); return (TK_MBR); }
"disk"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_DISK); }
"mkfs"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_MKFS); }
"login"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_LOGIN); }
"usr"                       { sscanf(yytext, "%s", yylval.TEXT); return (TK_USR); }
"pwd"                       { sscanf(yytext, "%s", yylval.TEXT); return (TK_PWD); }
"logout"                    { sscanf(yytext, "%s", yylval.TEXT); return (TK_LOGOUT); }
"mkgrp"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_MKGRP); }
"rmgrp"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_RMGRP); }
"mkusr"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_MKUSR); }
"grp"                       { sscanf(yytext, "%s", yylval.TEXT); return (TK_GRP); }
"rmusr"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_RMUSR); }
"chmod"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_CHMOD); }
"ugo"                       { sscanf(yytext, "%s", yylval.TEXT); return (TK_UGO); }
"r"                         { sscanf(yytext, "%s", yylval.TEXT); return (TK_R); }
"mkfile"                    { sscanf(yytext, "%s", yylval.TEXT); return (TK_MKFILE); }
"p"                         { sscanf(yytext, "%s", yylval.TEXT); return (TK_P); }
"cont"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_CONT); }
"cat"                       { sscanf(yytext, "%s", yylval.TEXT); return (TK_CAT); }
"file"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_FILE); }
"rem"                       { sscanf(yytext, "%s", yylval.TEXT); return (TK_REM); }
"edit"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_EDIT); }
"ren"                       { sscanf(yytext, "%s", yylval.TEXT); return (TK_REN); }
"mkdir"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_MKDIR); }
"cp"                        { sscanf(yytext, "%s", yylval.TEXT); return (TK_CP); }
"mv"                        { sscanf(yytext, "%s", yylval.TEXT); return (TK_MV); }
"dest"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_DEST); }
"find"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_FIND); }
"chown"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_CHOWN); }
"chgrp"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_CHGRP); }
"pause"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_PAUSE); }
"inode"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_INODE); }
"journaling"                { sscanf(yytext, "%s", yylval.TEXT); return (TK_JOURNALING); }
"block"                     { sscanf(yytext, "%s", yylval.TEXT); return (TK_BLOCK); }
"bm_inode"                  { sscanf(yytext, "%s", yylval.TEXT); return (TK_BM_INODE); }
"tree"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_TREE); }
"sb"                        { sscanf(yytext, "%s", yylval.TEXT); return (TK_SB); }
"ls"                        { sscanf(yytext, "%s", yylval.TEXT); return (TK_LS); }
"ruta"                      { sscanf(yytext, "%s", yylval.TEXT); return (TK_RUTA); }

[-]				            { sscanf(yytext, "%s", yylval.TEXT); return (TK_GUION); }
[=]				            { sscanf(yytext, "%s", yylval.TEXT); return (TK_IGUAL); }

{CADENA}		            { int j, n = strlen(yytext);
                                for(int i=j=0; i < n; i++){
                                    if(yytext[i] != 34){
                                        yytext[j++] = yytext[i];
                                    }
                                }
                              yytext[j] = '\0';
                              yylval.TEXT = strdup(yytext); return (TK_CADENA); }
{RUTA}          	        { yylval.TEXT = strdup(yytext); return (TK_RUTA); }
{CONTRA}          	        { yylval.TEXT = strdup(yytext); return (TK_CONTRA); }
{IDENT}                 	{ yylval.TEXT = strdup(yytext); return (TK_IDENTIFICADOR); }
{NOMBRE_FILE}                 	{ yylval.TEXT = strdup(yytext); return (TK_FILENAME); }
{NUM}				        { yylval.TEXT = strdup(yytext); return (TK_NUMBER); }
{NUMNEGATIVO}               { yylval.TEXT = strdup(yytext); return (TK_NUMBER_NEGATIVO); }
"\n"				        { linea++; yycolumn = 1;}

.				            { printf ("Palabra no reconocida en la linea %d -> %s\n", linea, yytext); }

%%

