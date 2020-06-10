#include "RMDisk.h"
#include <string>
using namespace std;
RMDisk::RMDisk(QString path)
{
    this->path = path;
}

void RMDisk::Ejecutar(){
        string ruta = path.toStdString();
        if(remove(ruta.c_str()) == 0){
            printf("El Disco fue eliminado satisfactoriamente!!\n");
        }else{
            printf("No se pudo eliminar el archivo!\n");
        }
}
