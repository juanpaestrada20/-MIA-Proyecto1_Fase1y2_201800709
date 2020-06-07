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
IDENT [a-zA-Z_][a-zA-Z_0-9]*
RUTA "/"[^\n\t\r" "]+
CADENA \"[^\"]*\"

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

[-]				            { sscanf(yytext, "%s", yylval.TEXT); return (TK_GUION); }
[=]				            { sscanf(yytext, "%s", yylval.TEXT); return (TK_IGUAL); }

{CADENA}		            { yylval.TEXT = strdup(yytext); return (TK_CADENA); }

{RUTA}          	        { yylval.TEXT = strdup(yytext); return (TK_RUTA); }

"\n"				        { linea++; yycolumn = 1;}

{IDENT}                 	{ yylval.TEXT = strdup(yytext); return (TK_IDENTIFICADOR); }

{NUM}				        { yylval.TEXT = strdup(yytext); return (TK_NUMBER); }

.				            { printf ("Error lexico en la linea %d -> %s", linea, yytext); }

%%

