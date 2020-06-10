#include "MKDisk.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <QStringList>
#include <libgen.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

MKDisk::MKDisk(int size, char fit, char unit, QString path)
{
    this->size = size;
    this->fit = fit;
    this->unit = unit;
    this->path = path;
}

void MKDisk::Ejecutar(){
    char *ruta = this->path.toLocal8Bit().data();
    VerificarDirectorio(ruta);
    this->MasterBootRecord.disk_signature = rand() % 100 + 1;
    this->MasterBootRecord.disk_fit = this->fit;
    this->MasterBootRecord.creation_time = time(0);
    for(int i = 0; i < 4; i++){
        this->MasterBootRecord.partitions[i].status = '0';
        this->MasterBootRecord.partitions[i].fit = '0';
        this->MasterBootRecord.partitions[i].size= 0;
        this->MasterBootRecord.partitions[i].start = 0;
    }
    int s;

    FILE *f =fopen(path.toStdString().c_str(), "wb");
    if(f == NULL)
    {
        printf("Error al crear el disco\n");
        return;
    }
    if(this->unit == 'k'){
        s = this->size*1024;
        this->MasterBootRecord.size = s;
    }else{
        s = this->size*1024*1024;
        this->MasterBootRecord.size = s;
    }
    rewind(f);
    fwrite(&MasterBootRecord,sizeof(MBR),1,f);
    fseek(f,MasterBootRecord.size-1,SEEK_SET);
    fwrite("\0",1,1,f);

    fclose(f);

    // Creacion de RAID
    string pathRaid = path.toStdString().substr(0, path.size()-5);

    ifstream src(path.toStdString(), std::ios::binary);
    string path2 = path.toStdString().substr(0, path.size()-5);
    std::ofstream  dst(path2+"_RAID.disk",   std::ios::binary);
    dst << src.rdbuf();
    EstadoMBR();

    printf("Se creo el disco de manera exitosa!\n");
}

void MKDisk::VerificarDirectorio(char* ruta){
    char delim[] = "/";
    char nuevo[1000];
    memset(nuevo,'\0',sizeof(nuevo));
    strcat(nuevo,ruta);

    char *ptr = strtok(nuevo,delim);


    char* carpetas[100];
    memset(carpetas,'\0',sizeof(carpetas));

    int size=0;
    while(ptr != NULL)
    {
        carpetas[size] = ptr;
        size++;
        ptr = strtok(NULL, delim);
    }

    char auxRuta[1000];
    memset(auxRuta,'\0',sizeof(auxRuta));
    auxRuta[0] = '/';

    for(int i=0; i<size-1 ;i++)
    {
        if(i != 0)
            strcat(auxRuta,"/");

        strcat(auxRuta,carpetas[i]);

        struct stat st = {0};
        string str1 ="";
        if(stat(auxRuta,&st) == -1){
            str1.append("mkdir -p \'");
            str1.append(auxRuta);
            str1.append("\'");
            system(str1.c_str());
            mkdir(auxRuta,0700);
        }

    }
}

void MKDisk::EstadoMBR(){
    printf("Creado Master Boot Record\n");
    printf("Tamaño: %i\n", this->MasterBootRecord.size);
    printf("Fit: %cf\n",this->fit);
    printf("Unidad: %c\n", this->unit);
    printf("Path: %s\n", this->path.toStdString().c_str());
}
