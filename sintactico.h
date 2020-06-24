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
    TK_NUMBER = 292,
    TK_NUMBER_NEGATIVO = 293,
    TK_LOGIN = 294,
    TK_MKFS = 295,
    TK_USR = 296,
    TK_PWD = 297,
    TK_LOGOUT = 298,
    TK_MKGRP = 299,
    TK_RMGRP = 300,
    TK_MKUSR = 301,
    TK_GRP = 302,
    TK_RMUSR = 303,
    TK_CHMOD = 304,
    TK_UGO = 305,
    TK_R = 306,
    TK_MKFILE = 307,
    TK_CONT = 308,
    TK_CAT = 309,
    TK_FILE = 310,
    TK_REM = 311,
    TK_EDIT = 312,
    TK_REN = 313,
    TK_MKDIR = 314,
    TK_CP = 315,
    TK_MV = 316,
    TK_DEST = 317,
    TK_FIND = 318,
    TK_CHOWN = 319,
    TK_CHGRP = 320,
    TK_PAUSE = 321,
    TK_INODE = 322,
    TK_JOURNALING = 323,
    TK_BLOCK = 324,
    TK_BM_INODE = 325,
    TK_BM_BLOCK = 326,
    TK_TREE = 327,
    TK_SB = 328,
    TK_LS = 329,
    TK_RUTA_REP = 330,
    TK_CONTRA = 331,
    TK_FILENAME = 332,
    TK_PUNTO = 333
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 148 "sintactico.y"

    char *TEXT;
    class NodoAST *NODO;

#line 141 "sintactico.h"

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
