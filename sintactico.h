/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SINTACTICO_H_INCLUDED
# define YY_YY_SINTACTICO_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_MKDISK = 258,
    TK_RMDISK = 259,
    TK_FDISK = 260,
    TK_MOUNT = 261,
    TK_UNMOUNT = 262,
    TK_REP = 263,
    TK_SIZE = 264,
    TK_FIT = 265,
    TK_UNIT = 266,
    TK_PATH = 267,
    TK_TYPE = 268,
    TK_DELETE = 269,
    TK_NAME = 270,
    TK_ADD = 271,
    TK_ID = 272,
    TK_EXEC = 273,
    TK_BESTFIT = 274,
    TK_FIRSTFIT = 275,
    TK_WORSTFIT = 276,
    TK_KYLOBYTES = 277,
    TK_MEGABYTES = 278,
    TK_BYTES = 279,
    TK_PRIMARIA = 280,
    TK_EXTENDIDA = 281,
    TK_LOGICA = 282,
    TK_FAST = 283,
    TK_FULL = 284,
    TK_MBR = 285,
    TK_DISK = 286,
    TK_GUION = 287,
    TK_IGUAL = 288,
    TK_CADENA = 289,
    TK_RUTA = 290,
    TK_IDENTIFICADOR = 291,
    TK_NUMBER = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 80 "sintactico.y"

    char *TEXT;
    class NodoAST *NODO;

#line 100 "sintactico.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SINTACTICO_H_INCLUDED  */
