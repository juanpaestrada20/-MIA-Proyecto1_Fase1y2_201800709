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

#endif // STRUCTURES_H
