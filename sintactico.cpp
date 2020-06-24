/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "sintactico.y"


#include <stdio.h>
#include <stdlib.h>
#include <string>
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

NodoAST *raiz = new NodoAST("INICIO");

#line 92 "sintactico.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 228 "sintactico.cpp"

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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  88
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   306

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  144
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  298

#define YYUNDEFTOK  2
#define YYMAXUTOK   333


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   157,   157,   160,   161,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   193,   194,   197,   198,   199,   200,   203,   204,
     205,   208,   209,   210,   213,   214,   217,   218,   219,   220,
     221,   222,   223,   224,   227,   228,   231,   232,   233,   236,
     237,   240,   241,   244,   245,   248,   249,   250,   251,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     267,   268,   269,   272,   273,   276,   277,   280,   281,   284,
     285,   288,   289,   290,   293,   294,   295,   296,   299,   300,
     303,   304,   307,   308,   309,   312,   313,   316,   317,   318,
     321,   322,   325,   326,   327,   328,   331,   332,   335,   336,
     339,   340,   343,   344,   348,   349,   352,   353,   356,   357,
     360,   361,   365,   366,   369,   370,   373,   374,   377,   378,
     379,   382,   383,   386,   387
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_MKDISK", "TK_RMDISK", "TK_FDISK",
  "TK_MOUNT", "TK_UNMOUNT", "TK_REP", "TK_SIZE", "TK_FIT", "TK_UNIT",
  "TK_PATH", "TK_TYPE", "TK_DELETE", "TK_NAME", "TK_ADD", "TK_ID",
  "TK_EXEC", "TK_BESTFIT", "TK_FIRSTFIT", "TK_WORSTFIT", "TK_KYLOBYTES",
  "TK_MEGABYTES", "TK_BYTES", "TK_PRIMARIA", "TK_EXTENDIDA", "TK_LOGICA",
  "TK_FAST", "TK_FULL", "TK_MBR", "TK_DISK", "TK_GUION", "TK_IGUAL",
  "TK_CADENA", "TK_RUTA", "TK_IDENTIFICADOR", "TK_NUMBER",
  "TK_NUMBER_NEGATIVO", "TK_LOGIN", "TK_MKFS", "TK_USR", "TK_PWD",
  "TK_LOGOUT", "TK_MKGRP", "TK_RMGRP", "TK_MKUSR", "TK_GRP", "TK_RMUSR",
  "TK_CHMOD", "TK_UGO", "TK_R", "TK_MKFILE", "TK_CONT", "TK_CAT",
  "TK_FILE", "TK_REM", "TK_EDIT", "TK_REN", "TK_MKDIR", "TK_CP", "TK_MV",
  "TK_DEST", "TK_FIND", "TK_CHOWN", "TK_CHGRP", "TK_PAUSE", "TK_INODE",
  "TK_JOURNALING", "TK_BLOCK", "TK_BM_INODE", "TK_BM_BLOCK", "TK_TREE",
  "TK_SB", "TK_LS", "TK_RUTA_REP", "TK_CONTRA", "TK_FILENAME", "TK_PUNTO",
  "$accept", "INICIO", "INSTRUCCIONES", "COMANDOS", "LISTAMKDISK",
  "OPCION_MKDISK", "TIPO_FIT", "TIPO_UNIT", "LISTAFDISK", "OPCION_FDISK",
  "TIPO_DELETE", "TIPO_PARTICION", "LISTAMOUNT", "OPCION_MOUNT",
  "LISTAREP", "OPCION_REP", "TIPO_REP", "TIPO_RUTA", "TIPO_NUMERO",
  "LISTAMKFS", "OPCION_MKFS", "LISTALOGIN", "OPCION_LOGIN", "TIPO_PWD",
  "TIPO_GRUPO", "LISTAMKUSR", "OPCION_MKUSR", "LISTACHMOD", "OPCION_CHMOD",
  "LISTAMKFILE", "OPCION_MKFILE", "LISTAEDIT", "OPCION_EDIT", "LISTAREN",
  "OPCION_REN", "LISTAMKDIR", "OPCION_MKDIR", "LISTACP", "OPCION_CP",
  "LISTAFIND", "OPCION_FIND", "LISTACHOWN", "OPCION_CHOWN", "LISTACHGRP",
  "OPCION_CHGRP", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333
};
# endif

#define YYPACT_NINF (-162)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -3,    12,    18,    84,    85,    91,   119,   123,   124,   125,
    -162,   127,   128,   129,   130,   131,   132,   133,   135,   136,
     137,   138,   139,   139,   140,   141,   142,  -162,    92,    -3,
    -162,   134,    12,  -162,   146,   126,    84,  -162,    56,    85,
    -162,   149,    -6,   119,  -162,   163,   -10,   124,  -162,    21,
     125,  -162,   161,   162,   -22,   129,  -162,   143,    23,   131,
    -162,    14,   132,  -162,   144,   166,     6,   136,  -162,    64,
     137,  -162,    -4,   138,  -162,     2,   139,  -162,   139,    65,
     140,  -162,    81,   141,  -162,   -25,   142,  -162,  -162,  -162,
     147,   148,   150,   152,  -162,   153,   154,   155,   156,   157,
     158,   159,   160,   164,  -162,   165,   167,  -162,   168,   169,
     170,   171,   172,  -162,    60,   173,   174,   175,  -162,   176,
     177,  -162,   178,   179,   180,   181,   182,  -162,   183,   184,
     185,  -162,  -162,   186,   187,  -162,   188,  -162,   189,   190,
     191,   192,  -162,   193,   194,  -162,   195,  -162,  -162,   196,
     197,  -162,   198,   199,  -162,   200,   201,  -162,  -162,   202,
     203,  -162,    45,     9,    97,    60,    60,    45,     9,    97,
      60,   100,    70,   113,    45,    60,   113,   113,    60,    17,
     113,    60,  -162,  -162,  -162,  -162,   113,   -24,   -24,    70,
     113,   113,   113,   113,   -24,   113,   113,    60,   145,   204,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,   113,   113,   113,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,  -162,  -162,  -162
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    31,     0,     2,
       4,     0,     5,    33,     0,     0,     7,    45,     0,     8,
      60,     0,     0,    10,    64,     0,     0,    13,    90,     0,
      12,    86,     0,     0,     0,    17,   101,     0,     0,    19,
     106,     0,    20,   111,     0,     0,     0,    23,   117,     0,
      24,   121,     0,    25,   125,     0,    26,   129,    27,     0,
      28,   133,     0,    29,   137,     0,    30,   142,     1,     3,
       0,     0,     0,     0,    32,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,     0,    59,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,    89,     0,
       0,    85,     0,     0,     0,     0,     0,   100,     0,     0,
       0,   109,   105,     0,     0,   115,     0,   110,     0,     0,
       0,     0,   116,     0,     0,   120,     0,   127,   124,     0,
       0,   128,     0,     0,   132,     0,     0,   140,   136,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,    80,    81,    11,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    34,
      38,    39,    40,    37,    42,    43,    41,    35,    36,     6,
      46,    50,    47,    48,    56,    57,    58,    49,    54,    55,
      51,    98,    99,    52,    53,    61,    62,     9,    66,    69,
      70,    78,    71,    72,    73,    74,    75,    76,    77,    79,
      65,    67,    68,    93,    94,    96,    97,    95,    91,    92,
      88,    87,    15,    16,   102,   103,   104,    18,   107,   108,
     113,   112,   114,    21,    22,   118,   119,   122,   123,   126,
     130,   131,   134,   135,   138,   139,   144,   143
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -162,  -162,  -162,   208,  -162,   206,    11,    25,  -162,   207,
       7,  -162,  -162,   205,  -162,   209,  -162,  -100,  -150,  -162,
     210,  -162,   211,  -161,   -62,  -162,   212,  -162,   213,  -162,
     214,  -162,   215,  -162,   216,  -162,   122,   217,    72,  -162,
     218,  -162,   219,  -162,   220
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    28,    29,    30,    32,    33,   223,   227,    36,    37,
     240,   237,    39,    40,    43,    44,   260,   185,   219,    50,
      51,    47,    48,   268,   243,    55,    56,    59,    60,    62,
      63,    67,    68,    70,    71,    73,    74,    76,    77,    80,
      81,    83,    84,    86,    87
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       1,     2,     3,     4,     5,     6,   109,   115,   146,   110,
     264,   111,   265,   266,   149,     7,   159,   230,   140,   124,
     125,   147,   160,   133,   244,   126,   134,   269,   220,   221,
     222,   116,   117,   275,   119,   129,     8,     9,   120,   135,
      10,    11,    12,    13,    31,    14,    15,   249,   250,    16,
      34,    17,   267,    18,    19,    20,    21,    22,    23,   141,
      24,    25,    26,    27,   150,   228,   229,   136,   105,   112,
     233,   106,   251,   130,   131,   245,   143,   152,   248,   144,
     153,   262,   217,   218,   252,   253,   254,   255,   256,   257,
     258,   259,    88,   155,   182,   183,   184,   278,   238,   239,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   246,   247,    35,    38,   261,   224,
     225,   226,   156,    41,   263,   234,   235,   236,   271,   272,
     273,   274,   157,   276,   277,    96,    97,    98,    99,   100,
     101,   102,   103,    90,    91,    92,    93,   241,   151,   242,
     151,    42,   295,   296,   297,    45,    46,    49,    95,    52,
      53,    54,    57,    58,    61,    64,   108,    65,    66,    69,
      72,    75,    79,    82,    85,   114,   122,   123,   139,   231,
     162,   163,   279,   164,   128,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   232,   148,   270,   174,   175,   138,
     176,   177,   178,   179,   180,   181,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,    89,    94,     0,
      78,   280,     0,   104,   107,     0,     0,     0,     0,     0,
       0,     0,   113,     0,     0,     0,     0,     0,   118,     0,
     121,     0,     0,     0,     0,     0,     0,   127,     0,     0,
       0,     0,   132,     0,     0,     0,   137,     0,     0,     0,
       0,     0,   142,     0,     0,     0,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
       0,     0,   158,     0,     0,     0,   161
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,     6,     7,     8,    12,    17,    12,    15,
      34,    17,    36,    37,    12,    18,    41,   167,    12,    41,
      42,    25,    47,     9,   174,    47,    12,   188,    19,    20,
      21,    41,    42,   194,    13,    12,    39,    40,    17,    25,
      43,    44,    45,    46,    32,    48,    49,    30,    31,    52,
      32,    54,    76,    56,    57,    58,    59,    60,    61,    53,
      63,    64,    65,    66,    62,   165,   166,    53,    12,    75,
     170,    15,    55,    50,    51,   175,    12,    12,   178,    15,
      15,   181,    37,    38,    67,    68,    69,    70,    71,    72,
      73,    74,     0,    12,    34,    35,    36,   197,    28,    29,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   176,   177,    32,    32,   180,    22,
      23,    24,    41,    32,   186,    25,    26,    27,   190,   191,
     192,   193,    51,   195,   196,     9,    10,    11,    12,    13,
      14,    15,    16,     9,    10,    11,    12,    34,    76,    36,
      78,    32,   214,   215,   216,    32,    32,    32,    12,    32,
      32,    32,    32,    32,    32,    32,    17,    32,    32,    32,
      32,    32,    32,    32,    32,    12,    15,    15,    12,   168,
      33,    33,    37,    33,    41,    33,    33,    33,    33,    33,
      33,    33,    33,    33,   169,    73,   189,    33,    33,    55,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    29,    32,    -1,
      23,    37,    -1,    36,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    86
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    18,    39,    40,
      43,    44,    45,    46,    48,    49,    52,    54,    56,    57,
      58,    59,    60,    61,    63,    64,    65,    66,    80,    81,
      82,    32,    83,    84,    32,    32,    87,    88,    32,    91,
      92,    32,    32,    93,    94,    32,    32,   100,   101,    32,
      98,    99,    32,    32,    32,   104,   105,    32,    32,   106,
     107,    32,   108,   109,    32,    32,    32,   110,   111,    32,
     112,   113,    32,   114,   115,    32,   116,   117,   116,    32,
     118,   119,    32,   120,   121,    32,   122,   123,     0,    82,
       9,    10,    11,    12,    84,    12,     9,    10,    11,    12,
      13,    14,    15,    16,    88,    12,    15,    92,    17,    12,
      15,    17,    75,    94,    12,    17,    41,    42,   101,    13,
      17,    99,    15,    15,    41,    42,    47,   105,    41,    12,
      50,    51,   107,     9,    12,    25,    53,   109,    55,    12,
      12,    53,   111,    12,    15,   113,    12,    25,   115,    12,
      62,   117,    12,    15,   119,    12,    41,    51,   121,    41,
      47,   123,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    34,    35,    36,    96,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    37,    38,    97,
      19,    20,    21,    85,    22,    23,    24,    86,    96,    96,
      97,    85,    86,    96,    25,    26,    27,    90,    28,    29,
      89,    34,    36,   103,    97,    96,   103,   103,    96,    30,
      31,    55,    67,    68,    69,    70,    71,    72,    73,    74,
      95,   103,    96,   103,    34,    36,    37,    76,   102,   102,
      89,   103,   103,   103,   103,   102,   103,   103,    96,    37,
      37,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,   103,   103,   103
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    79,    80,    81,    81,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    83,    83,    84,    84,    84,    84,    85,    85,
      85,    86,    86,    86,    87,    87,    88,    88,    88,    88,
      88,    88,    88,    88,    89,    89,    90,    90,    90,    91,
      91,    92,    92,    93,    93,    94,    94,    94,    94,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      96,    96,    96,    97,    97,    98,    98,    99,    99,   100,
     100,   101,   101,   101,   102,   102,   102,   102,   103,   103,
     104,   104,   105,   105,   105,   106,   106,   107,   107,   107,
     108,   108,   109,   109,   109,   109,   110,   110,   111,   111,
     112,   112,   113,   113,   114,   114,   115,   115,   116,   116,
     117,   117,   118,   118,   119,   119,   120,   120,   121,   121,
     121,   122,   122,   123,   123
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     5,     2,     2,     5,
       2,     4,     2,     2,     1,     5,     5,     2,     5,     2,
       2,     5,     5,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     2,     1,     4,     4,     4,     4,     1,     1,
       1,     1,     1,     1,     2,     1,     4,     4,     4,     4,
       4,     4,     4,     4,     1,     1,     1,     1,     1,     2,
       1,     4,     4,     2,     1,     4,     4,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     4,     4,     2,
       1,     4,     4,     4,     1,     1,     1,     1,     1,     1,
       2,     1,     4,     4,     4,     2,     1,     4,     4,     2,
       2,     1,     4,     4,     4,     2,     2,     1,     4,     4,
       2,     1,     4,     4,     2,     1,     4,     2,     2,     1,
       4,     4,     2,     1,     4,     4,     2,     1,     4,     4,
       2,     2,     1,     4,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 157 "sintactico.y"
                      { raiz=(yyval.NODO);}
#line 1736 "sintactico.cpp"
    break;

  case 3:
#line 160 "sintactico.y"
                                      { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 1742 "sintactico.cpp"
    break;

  case 4:
#line 161 "sintactico.y"
                                      { (yyval.NODO) = new NodoAST("INSTRUCCION"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 1748 "sintactico.cpp"
    break;

  case 5:
#line 164 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("MkDisk"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1754 "sintactico.cpp"
    break;

  case 6:
#line 165 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("RmDisk"); NodoAST *n2 = new NodoAST("Path"); NodoAST *n5 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n5); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 1760 "sintactico.cpp"
    break;

  case 7:
#line 166 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("FDisk"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1766 "sintactico.cpp"
    break;

  case 8:
#line 167 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Mount"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1772 "sintactico.cpp"
    break;

  case 9:
#line 168 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Unmount"); NodoAST *n2 = new NodoAST("ID"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n4); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 1778 "sintactico.cpp"
    break;

  case 10:
#line 169 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Rep"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1784 "sintactico.cpp"
    break;

  case 11:
#line 170 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Exec"); NodoAST *n2 = new NodoAST("Path"); NodoAST *n5 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n5); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 1790 "sintactico.cpp"
    break;

  case 12:
#line 171 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("MkFS"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1796 "sintactico.cpp"
    break;

  case 13:
#line 172 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Login"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1;}
#line 1802 "sintactico.cpp"
    break;

  case 14:
#line 173 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Logout"); (yyval.NODO) = n1; }
#line 1808 "sintactico.cpp"
    break;

  case 15:
#line 174 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("MkGrp"); NodoAST *n2 = new NodoAST("Name"); NodoAST *n3 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n3); n1->agregarHijo(*n2); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 1814 "sintactico.cpp"
    break;

  case 16:
#line 175 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("RmGrp"); NodoAST *n2 = new NodoAST("Name"); NodoAST *n3 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n3); n1->agregarHijo(*n2); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 1820 "sintactico.cpp"
    break;

  case 17:
#line 176 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("MkUsr"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1826 "sintactico.cpp"
    break;

  case 18:
#line 177 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("RmUsr"); NodoAST *n2 = new NodoAST("Usr"); NodoAST *n3 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n3); n1->agregarHijo(*n2); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 1832 "sintactico.cpp"
    break;

  case 19:
#line 178 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Chmod"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1838 "sintactico.cpp"
    break;

  case 20:
#line 179 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("MkFile"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1844 "sintactico.cpp"
    break;

  case 21:
#line 180 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Cat"); NodoAST *n2 = new NodoAST("File"); NodoAST *n3 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n3); n1->agregarHijo(*n2); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 1850 "sintactico.cpp"
    break;

  case 22:
#line 181 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Rem"); NodoAST *n2 = new NodoAST("Path"); NodoAST *n3 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n3); n1->agregarHijo(*n2); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 1856 "sintactico.cpp"
    break;

  case 23:
#line 182 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Edit"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1862 "sintactico.cpp"
    break;

  case 24:
#line 183 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Ren"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1868 "sintactico.cpp"
    break;

  case 25:
#line 184 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("MkDir"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1874 "sintactico.cpp"
    break;

  case 26:
#line 185 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Cp"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1880 "sintactico.cpp"
    break;

  case 27:
#line 186 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Mv"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1886 "sintactico.cpp"
    break;

  case 28:
#line 187 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Find"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1892 "sintactico.cpp"
    break;

  case 29:
#line 188 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Chown"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1898 "sintactico.cpp"
    break;

  case 30:
#line 189 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("ChGrp"); n1->agregarHijo(*(yyvsp[0].NODO)); (yyval.NODO) = n1; }
#line 1904 "sintactico.cpp"
    break;

  case 31:
#line 190 "sintactico.y"
                                                                { NodoAST *n1 = new NodoAST("Pause"); (yyval.NODO) = n1; }
#line 1910 "sintactico.cpp"
    break;

  case 32:
#line 193 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 1916 "sintactico.cpp"
    break;

  case 33:
#line 194 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 1922 "sintactico.cpp"
    break;

  case 34:
#line 197 "sintactico.y"
                                                      { NodoAST *n2 = new NodoAST("Size"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n4); (yyval.NODO) = n2; }
#line 1928 "sintactico.cpp"
    break;

  case 35:
#line 198 "sintactico.y"
                                                      { NodoAST *n2 = new NodoAST("Unit"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n4); (yyval.NODO) = n2; }
#line 1934 "sintactico.cpp"
    break;

  case 36:
#line 199 "sintactico.y"
                                                      { NodoAST *n2 = new NodoAST("Ruta"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n4); (yyval.NODO) = n2; }
#line 1940 "sintactico.cpp"
    break;

  case 37:
#line 200 "sintactico.y"
                                                      { NodoAST *n2 = new NodoAST("Fit");  NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n2->agregarHijo(*n4); (yyval.NODO) = n2; }
#line 1946 "sintactico.cpp"
    break;

  case 38:
#line 203 "sintactico.y"
                        { (yyval.TEXT) = "bf"; }
#line 1952 "sintactico.cpp"
    break;

  case 39:
#line 204 "sintactico.y"
                        { (yyval.TEXT) = "ff"; }
#line 1958 "sintactico.cpp"
    break;

  case 40:
#line 205 "sintactico.y"
                        { (yyval.TEXT) = "wf"; }
#line 1964 "sintactico.cpp"
    break;

  case 41:
#line 208 "sintactico.y"
                        { (yyval.TEXT) = "b"; }
#line 1970 "sintactico.cpp"
    break;

  case 42:
#line 209 "sintactico.y"
                        { (yyval.TEXT) = "k"; }
#line 1976 "sintactico.cpp"
    break;

  case 43:
#line 210 "sintactico.y"
                        { (yyval.TEXT) = "m"; }
#line 1982 "sintactico.cpp"
    break;

  case 44:
#line 213 "sintactico.y"
                                    { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 1988 "sintactico.cpp"
    break;

  case 45:
#line 214 "sintactico.y"
                                    { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 1994 "sintactico.cpp"
    break;

  case 46:
#line 217 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Size"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2000 "sintactico.cpp"
    break;

  case 47:
#line 218 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Unit"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2006 "sintactico.cpp"
    break;

  case 48:
#line 219 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Path"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2012 "sintactico.cpp"
    break;

  case 49:
#line 220 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Type"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2018 "sintactico.cpp"
    break;

  case 50:
#line 221 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Fit"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2024 "sintactico.cpp"
    break;

  case 51:
#line 222 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Delete"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2030 "sintactico.cpp"
    break;

  case 52:
#line 223 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Name"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2036 "sintactico.cpp"
    break;

  case 53:
#line 224 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Add"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2042 "sintactico.cpp"
    break;

  case 54:
#line 227 "sintactico.y"
                        { (yyval.TEXT) = "Fast"; }
#line 2048 "sintactico.cpp"
    break;

  case 55:
#line 228 "sintactico.y"
                        { (yyval.TEXT) = "Full"; }
#line 2054 "sintactico.cpp"
    break;

  case 56:
#line 231 "sintactico.y"
                                { (yyval.TEXT) = "Primaria"; }
#line 2060 "sintactico.cpp"
    break;

  case 57:
#line 232 "sintactico.y"
                                { (yyval.TEXT) = "Extendida"; }
#line 2066 "sintactico.cpp"
    break;

  case 58:
#line 233 "sintactico.y"
                                { (yyval.TEXT) = "Logica"; }
#line 2072 "sintactico.cpp"
    break;

  case 59:
#line 236 "sintactico.y"
                                    { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2078 "sintactico.cpp"
    break;

  case 60:
#line 237 "sintactico.y"
                                    { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2084 "sintactico.cpp"
    break;

  case 61:
#line 240 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Path"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2090 "sintactico.cpp"
    break;

  case 62:
#line 241 "sintactico.y"
                                                            { NodoAST *n1 = new NodoAST("Name"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2096 "sintactico.cpp"
    break;

  case 63:
#line 244 "sintactico.y"
                                { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2102 "sintactico.cpp"
    break;

  case 64:
#line 245 "sintactico.y"
                                { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2108 "sintactico.cpp"
    break;

  case 65:
#line 248 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Name"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2114 "sintactico.cpp"
    break;

  case 66:
#line 249 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Path"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2120 "sintactico.cpp"
    break;

  case 67:
#line 250 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("ID"); NodoAST *n4 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n4); (yyval.NODO) = n1; }
#line 2126 "sintactico.cpp"
    break;

  case 68:
#line 251 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Ruta"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2132 "sintactico.cpp"
    break;

  case 69:
#line 254 "sintactico.y"
                        { (yyval.TEXT) = "MBR";}
#line 2138 "sintactico.cpp"
    break;

  case 70:
#line 255 "sintactico.y"
                        { (yyval.TEXT) = "Disk"; }
#line 2144 "sintactico.cpp"
    break;

  case 71:
#line 256 "sintactico.y"
                        { (yyval.TEXT) = "Inode"; }
#line 2150 "sintactico.cpp"
    break;

  case 72:
#line 257 "sintactico.y"
                        { (yyval.TEXT) = "Journaling"; }
#line 2156 "sintactico.cpp"
    break;

  case 73:
#line 258 "sintactico.y"
                        { (yyval.TEXT) = "Block"; }
#line 2162 "sintactico.cpp"
    break;

  case 74:
#line 259 "sintactico.y"
                        { (yyval.TEXT) = "BMInode"; }
#line 2168 "sintactico.cpp"
    break;

  case 75:
#line 260 "sintactico.y"
                        { (yyval.TEXT) = "BMBlock"; }
#line 2174 "sintactico.cpp"
    break;

  case 76:
#line 261 "sintactico.y"
                        { (yyval.TEXT) = "Tree"; }
#line 2180 "sintactico.cpp"
    break;

  case 77:
#line 262 "sintactico.y"
                        { (yyval.TEXT) = "SB"; }
#line 2186 "sintactico.cpp"
    break;

  case 78:
#line 263 "sintactico.y"
                        { (yyval.TEXT) = "File"; }
#line 2192 "sintactico.cpp"
    break;

  case 79:
#line 264 "sintactico.y"
                        { (yyval.TEXT) = "LS"; }
#line 2198 "sintactico.cpp"
    break;

  case 80:
#line 267 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2204 "sintactico.cpp"
    break;

  case 81:
#line 268 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2210 "sintactico.cpp"
    break;

  case 82:
#line 269 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2216 "sintactico.cpp"
    break;

  case 83:
#line 272 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2222 "sintactico.cpp"
    break;

  case 84:
#line 273 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2228 "sintactico.cpp"
    break;

  case 85:
#line 276 "sintactico.y"
                                  { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2234 "sintactico.cpp"
    break;

  case 86:
#line 277 "sintactico.y"
                                  { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2240 "sintactico.cpp"
    break;

  case 87:
#line 280 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("ID"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2246 "sintactico.cpp"
    break;

  case 88:
#line 281 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Type"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2252 "sintactico.cpp"
    break;

  case 89:
#line 284 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2258 "sintactico.cpp"
    break;

  case 90:
#line 285 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2264 "sintactico.cpp"
    break;

  case 91:
#line 288 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Usr"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2270 "sintactico.cpp"
    break;

  case 92:
#line 289 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Pwd"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2276 "sintactico.cpp"
    break;

  case 93:
#line 290 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("ID"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2282 "sintactico.cpp"
    break;

  case 94:
#line 293 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2288 "sintactico.cpp"
    break;

  case 95:
#line 294 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2294 "sintactico.cpp"
    break;

  case 96:
#line 295 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2300 "sintactico.cpp"
    break;

  case 97:
#line 296 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2306 "sintactico.cpp"
    break;

  case 98:
#line 299 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2312 "sintactico.cpp"
    break;

  case 99:
#line 300 "sintactico.y"
                                { (yyval.TEXT) = (yyvsp[0].TEXT); }
#line 2318 "sintactico.cpp"
    break;

  case 100:
#line 303 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2324 "sintactico.cpp"
    break;

  case 101:
#line 304 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2330 "sintactico.cpp"
    break;

  case 102:
#line 307 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Usr"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2336 "sintactico.cpp"
    break;

  case 103:
#line 308 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Pwd"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2342 "sintactico.cpp"
    break;

  case 104:
#line 309 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Grp"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2348 "sintactico.cpp"
    break;

  case 105:
#line 312 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2354 "sintactico.cpp"
    break;

  case 106:
#line 313 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2360 "sintactico.cpp"
    break;

  case 107:
#line 316 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Path"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2366 "sintactico.cpp"
    break;

  case 108:
#line 317 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Ugo"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2372 "sintactico.cpp"
    break;

  case 109:
#line 318 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("R"); (yyval.NODO) = n1; }
#line 2378 "sintactico.cpp"
    break;

  case 110:
#line 321 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2384 "sintactico.cpp"
    break;

  case 111:
#line 322 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2390 "sintactico.cpp"
    break;

  case 112:
#line 325 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Path"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2396 "sintactico.cpp"
    break;

  case 113:
#line 326 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Size"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2402 "sintactico.cpp"
    break;

  case 114:
#line 327 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Cont"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2408 "sintactico.cpp"
    break;

  case 115:
#line 328 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("P"); (yyval.NODO) = n1; }
#line 2414 "sintactico.cpp"
    break;

  case 116:
#line 331 "sintactico.y"
                                 { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2420 "sintactico.cpp"
    break;

  case 117:
#line 332 "sintactico.y"
                                 { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2426 "sintactico.cpp"
    break;

  case 118:
#line 335 "sintactico.y"
                                                 { NodoAST *n1 = new NodoAST("Path"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2432 "sintactico.cpp"
    break;

  case 119:
#line 336 "sintactico.y"
                                                 { NodoAST *n1 = new NodoAST("Cont"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2438 "sintactico.cpp"
    break;

  case 120:
#line 339 "sintactico.y"
                                { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2444 "sintactico.cpp"
    break;

  case 121:
#line 340 "sintactico.y"
                                { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2450 "sintactico.cpp"
    break;

  case 122:
#line 343 "sintactico.y"
                                                  { NodoAST *n1 = new NodoAST("Path"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2456 "sintactico.cpp"
    break;

  case 123:
#line 344 "sintactico.y"
                                                  { NodoAST *n1 = new NodoAST("Name"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2462 "sintactico.cpp"
    break;

  case 124:
#line 348 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2468 "sintactico.cpp"
    break;

  case 125:
#line 349 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2474 "sintactico.cpp"
    break;

  case 126:
#line 352 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Path"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2480 "sintactico.cpp"
    break;

  case 127:
#line 353 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("P"); (yyval.NODO) = n1; }
#line 2486 "sintactico.cpp"
    break;

  case 128:
#line 356 "sintactico.y"
                                { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2492 "sintactico.cpp"
    break;

  case 129:
#line 357 "sintactico.y"
                                { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2498 "sintactico.cpp"
    break;

  case 130:
#line 360 "sintactico.y"
                                               { NodoAST *n1 = new NodoAST("Path"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2504 "sintactico.cpp"
    break;

  case 131:
#line 361 "sintactico.y"
                                               { NodoAST *n1 = new NodoAST("Dest"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2510 "sintactico.cpp"
    break;

  case 132:
#line 365 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2516 "sintactico.cpp"
    break;

  case 133:
#line 366 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2522 "sintactico.cpp"
    break;

  case 134:
#line 369 "sintactico.y"
                                                    { NodoAST *n1 = new NodoAST("Path"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2528 "sintactico.cpp"
    break;

  case 135:
#line 370 "sintactico.y"
                                                    { NodoAST *n1 = new NodoAST("Ruta"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2534 "sintactico.cpp"
    break;

  case 136:
#line 373 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2540 "sintactico.cpp"
    break;

  case 137:
#line 374 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2546 "sintactico.cpp"
    break;

  case 138:
#line 377 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Path"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2552 "sintactico.cpp"
    break;

  case 139:
#line 378 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Usr"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2558 "sintactico.cpp"
    break;

  case 140:
#line 379 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("R"); (yyval.NODO) = n1; }
#line 2564 "sintactico.cpp"
    break;

  case 141:
#line 382 "sintactico.y"
                                        { (yyval.NODO) = (yyvsp[-1].NODO); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2570 "sintactico.cpp"
    break;

  case 142:
#line 383 "sintactico.y"
                                        { (yyval.NODO) = new NodoAST("Parametros"); (yyval.NODO)->agregarHijo(*(yyvsp[0].NODO)); }
#line 2576 "sintactico.cpp"
    break;

  case 143:
#line 386 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Grp"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2582 "sintactico.cpp"
    break;

  case 144:
#line 387 "sintactico.y"
                                                        { NodoAST *n1 = new NodoAST("Ur"); NodoAST *n2 = new NodoAST((yyvsp[0].TEXT)); n1->agregarHijo(*n2); (yyval.NODO) = n1; }
#line 2588 "sintactico.cpp"
    break;


#line 2592 "sintactico.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 391 "sintactico.y"

