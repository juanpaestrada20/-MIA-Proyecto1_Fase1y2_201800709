#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
using namespace std;

typedef struct Partition{
    char status;
    char type;
    char fit;
    int start;
    int size;
    char name[16];
} MBR_Part;

typedef struct MasterBootRecord{
    int size;
    time_t creation_time;
    int  disk_signature;
    char disk_fit;
    MBR_Part partitions[4];
} MBR;

typedef struct ExtendedBootRecord{
    char status;
    char fit;
    int start;
    int size;
    int next;
    char name[16];
} EBR;


typedef struct SuperBloque{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}SuperBloque;

typedef struct InodoTable{
    int i_uid;
    int i_gid;
    int i_size;
    int i_block[15];
    char i_type;
    int i_perm;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
}Inodo;

typedef struct Content{
    char b_name[12];
    int b_inodo;
}Content;

typedef struct BloqueCarpeta{
    Content b_content[4];
}BloqueCarpeta;

typedef struct BloqueArchivo{
    char b_content[64];
}BloqueArchivo;

typedef struct BloqueApuntadores{
    int b_pointer[16];
}BloqueApuntadores;

typedef struct Journal{
    time_t journal_date;
    char user[12];
    char operation[256];
    char journal_operation_type[10];
    char journal_name[10];
    int journal_type;
    int journal_owner;
    int journal_permissions;
} Journal;


typedef struct Usuario{
    int id_usr;
    int id_grp;
    char username[12];
    char password[12];
    char group[12];
    char id[12];
}Usuario;

typedef struct UsuarioLogguedo{
    int id_user;
    int id_grp;
    int inicioSuper;
    int inicioJournal;
    int tipo_sistema;
    string direccion;
    char fit;
}Sesion;

typedef struct Files{
    string path;
    string cont;
    string texto;
    int size;
}Files;


#endif // STRUCTURES_H
