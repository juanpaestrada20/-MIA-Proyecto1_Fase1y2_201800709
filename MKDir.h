#ifndef MKDIR_H
#define MKDIR_H
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

class MKDir
{
public:
    string path;
    bool carpetasPadre;
    int permiso;

    MKDir();
};

#endif // MKDIR_H
